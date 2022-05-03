/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2014-2021 NVIDIA CORPORATION
 * SPDX-License-Identifier: Apache-2.0
 */


// ImGui - standalone example application for Glfw + Vulkan, using programmable
// pipeline If you are new to ImGui, see examples/README.txt and documentation
// at the top of imgui.cpp.

#include <array>

#include "backends/imgui_impl_glfw.h"
#include "imgui.h"

#include "engine.h"
#include "imgui/imgui_camera_widget.h"
#include "nvh/cameramanipulator.hpp"
#include "nvh/fileoperations.hpp"
#include "nvpsystem.hpp"
#include "nvvk/commands_vk.hpp"
#include "nvvk/context_vk.hpp"

#include "scenes.hpp"


// ────────────────────────────────────────────────────────────────────────────────
#define UNUSED(x) (void)(x)
// ────────────────────────────────────────────────────────────────────────────────


// Default search path for shaders
std::vector<std::string> defaultSearchPaths;


// GLFW Callback functions
static void onErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


// Extra UI
void renderUI(Engine& engine) {
    bool changed = false;

    changed |= ImGuiH::CameraWidget();


    if (ImGui::CollapsingHeader("Light")) {
        auto& pc = engine.m_pcRaster;

        changed |= ImGui::RadioButton("Point", &pc.light_type, 0);
        ImGui::SameLine();
        changed |= ImGui::RadioButton("Infinite", &pc.light_type, 1);

        changed |= ImGui::SliderFloat3("Position", &pc.light_position.x, -20.f, 20.f);
        changed |= ImGui::SliderFloat("Intensity", &pc.light_intensity, 0.f, 150.f);
    }

    if (ImGui::CollapsingHeader("Ray tracing options")) {
        changed |= ImGui::SliderInt("Max depth of ray", &engine.m_pcRay.max_depth, 1, 50);
        changed |= ImGui::SliderInt("Max accum frames", &engine.m_maxAcumFrames, 1, 200);
        changed |= ImGui::SliderInt("Number of samples", &engine.m_pcRay.nb_samples, 1, 20);
    }

    if (changed) {
        engine.resetFrame();
    }
}


int main(int argc, char** argv) {
    UNUSED(argc);

    // Setup GLFW window
    glfwSetErrorCallback(onErrorCallback);

    if(!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(SAMPLE_WIDTH, SAMPLE_HEIGHT, PROJECT_NAME, nullptr, nullptr);

    CameraManip.setWindowSize(SAMPLE_WIDTH, SAMPLE_HEIGHT);

    // Setup Vulkan
    if(!glfwVulkanSupported()) {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }

    // setup some basic things for the sample, logging file for example
    NVPSystem system(PROJECT_NAME);

    // Search path for shaders and other media
    defaultSearchPaths = {
        NVPSystem::exePath() + PROJECT_RELDIRECTORY,
        NVPSystem::exePath() + PROJECT_RELDIRECTORY "..",
        std::string(PROJECT_NAME),
    };

    // Vulkan required extensions
    assert(glfwVulkanSupported() == 1);
    uint32_t count{0};
    auto     reqExtensions = glfwGetRequiredInstanceExtensions(&count);

    // Requesting Vulkan extensions and layers
    nvvk::ContextCreateInfo contextInfo;
    contextInfo.setVersion(1, 2);                       // Using Vulkan 1.2
    for(uint32_t ext_id = 0; ext_id < count; ext_id++)  // Adding required extensions (surface, win32, linux, ..)
        contextInfo.addInstanceExtension(reqExtensions[ext_id]);
    contextInfo.addInstanceLayer("VK_LAYER_LUNARG_monitor", true);              // FPS in titlebar
    contextInfo.addInstanceExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, true);  // Allow debug names
    contextInfo.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);            // Enabling ability to present rendering

    // #VkRay: activar las extensiones necesarias para ray tracing
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeature {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR
    };
    contextInfo.addDeviceExtension(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, false, &accelFeature);    // Para construir las estructuras de aceleración
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeature {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR
    };
    contextInfo.addDeviceExtension(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, false, &rtPipelineFeature);  // Para construir vkCmdTraceRaysKHR
    contextInfo.addDeviceExtension(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);  // Requisito del pipeline de ray tracing

    // Creating Vulkan base application
    nvvk::Context vkctx{};
    vkctx.initInstance(contextInfo);
    // Find all compatible devices
    auto compatibleDevices = vkctx.getCompatibleDevices(contextInfo);
    assert(!compatibleDevices.empty());
    // Use a compatible device
    vkctx.initDevice(compatibleDevices[0], contextInfo);

    // Initialize engine
    Engine engine;

    // Window need to be opened to get the surface on which to draw
    const VkSurfaceKHR surface = engine.getVkSurface(vkctx.m_instance, window);
    vkctx.setGCTQueueWithPresent(surface);

    engine.setup(vkctx.m_instance, vkctx.m_device, vkctx.m_physicalDevice, vkctx.m_queueGCT.familyIndex);
    engine.createSwapchain(surface, SAMPLE_WIDTH, SAMPLE_HEIGHT);
    engine.createDepthBuffer();
    engine.createRenderPass();
    engine.createFrameBuffers();

    // Setup Imgui
    engine.initGUI(0);  // Using sub-pass 0

    load_scene(Scene::cube_reflective, engine);

    engine.createOffscreenRender();
    engine.createDescriptorSetLayout();
    engine.createGraphicsPipeline();
    engine.createUniformBuffer();
    engine.createObjDescriptionBuffer();
    engine.updateDescriptorSet();

    // #VkRay
    engine.initRayTracing();
    engine.createBottomLevelAS();
    engine.createTopLevelAS();
    engine.createRtDescriptorSet();
    engine.createRtPipeline();
    engine.createRtShaderBindingTable();

    bool useRaytracer = true;

    engine.createPostDescriptor();
    engine.createPostPipeline();
    engine.updatePostDescriptorSet();
    nvmath::vec4f clear_color = nvmath::vec4f(1, 1, 1, 1.00f);


    engine.setupGlfwCallbacks(window);
    ImGui_ImplGlfw_InitForVulkan(window, true);

//
// ──────────────────────────────────────────────────────────────── MAIN LOOP ─────
//

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if(engine.isMinimized())
            continue;

        // Start the Dear ImGui frame
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show UI window.
        if(engine.showGui()) {
            ImGuiH::Panel::Begin();
            ImGui::ColorEdit3("Clear color", reinterpret_cast<float*>(&clear_color));
            ImGui::Checkbox("Ray tracer mode", &useRaytracer);  // Para cambiar entre raster y ray tracing.
            renderUI(engine);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGuiH::Control::Info("", "", "(F10) Toggle Pane", ImGuiH::Control::Flags::Disabled);
            ImGui::Text("Total samples = %i", engine.m_maxAcumFrames * engine.m_pcRay.nb_samples);
            ImGuiH::Panel::End();
        }

        // Start rendering the scene
        engine.prepareFrame();

        // Start command buffer of this frame
        auto                   curFrame = engine.getCurFrame();
        const VkCommandBuffer& cmdBuf   = engine.getCommandBuffers()[curFrame];

        VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmdBuf, &beginInfo);

        // Updating camera buffer
        engine.updateUniformBuffer(cmdBuf);

        // Clearing screen
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = {{clear_color[0], clear_color[1], clear_color[2], clear_color[3]}};
        clearValues[1].depthStencil = {1.0f, 0};

        // Offscreen render pass
        {
            VkRenderPassBeginInfo offscreenRenderPassBeginInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
            offscreenRenderPassBeginInfo.clearValueCount = 2;
            offscreenRenderPassBeginInfo.pClearValues    = clearValues.data();
            offscreenRenderPassBeginInfo.renderPass      = engine.m_offscreenRenderPass;
            offscreenRenderPassBeginInfo.framebuffer     = engine.m_offscreenFramebuffer;
            offscreenRenderPassBeginInfo.renderArea      = {{0, 0}, engine.getSize()};

            // Rendering Scene
            if (useRaytracer) {
                engine.raytrace(cmdBuf, clear_color);
            }
            else {
                vkCmdBeginRenderPass(cmdBuf, &offscreenRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                engine.rasterize(cmdBuf);
                vkCmdEndRenderPass(cmdBuf);
            }
        }


        // 2nd rendering pass: tone mapper, UI
        {
            VkRenderPassBeginInfo postRenderPassBeginInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
            postRenderPassBeginInfo.clearValueCount = 2;
            postRenderPassBeginInfo.pClearValues    = clearValues.data();
            postRenderPassBeginInfo.renderPass      = engine.getRenderPass();
            postRenderPassBeginInfo.framebuffer     = engine.getFramebuffers()[curFrame];
            postRenderPassBeginInfo.renderArea      = {{0, 0}, engine.getSize()};

            // Rendering tonemapper
            vkCmdBeginRenderPass(cmdBuf, &postRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            engine.drawPost(cmdBuf);
            // Rendering UI
            ImGui::Render();
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuf);
            vkCmdEndRenderPass(cmdBuf);
        }

        // Submit for display
        vkEndCommandBuffer(cmdBuf);
        engine.submitFrame();
    }

    // Cleanup
    vkDeviceWaitIdle(engine.getDevice());

    engine.destroyResources();
    engine.destroy();
    vkctx.deinit();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
