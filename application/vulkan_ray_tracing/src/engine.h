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

#pragma once

#include "nvvk/appbase_vk.hpp"
#include "nvvk/debug_util_vk.hpp"
#include "nvvk/descriptorsets_vk.hpp"
#include "nvvk/memallocator_dma_vk.hpp"
#include "nvvk/resourceallocator_vk.hpp"

#include "nvvk/raytraceKHR_vk.hpp"

#include "shaders/host_device.h"
#include "globals.hpp"


//--------------------------------------------------------------------------------------------------
// Simple rasterizer of OBJ objects
// - Each OBJ loaded are stored in an `ObjModel` and referenced by a `ObjInstance`
// - It is possible to have many `ObjInstance` referencing the same `ObjModel`
// - Rendering is done in an offscreen framebuffer
// - The image of the framebuffer is displayed in post-process in a full-screen quad
//
class Engine: public nvvk::AppBaseVk {
public:

// ─────────────────────────────────────────────────────────────────── MODELS ─────

    // Array of objects and instances in the scene
    std::vector<ObjModel>    m_objModel;   // Model on host
    std::vector<ObjDesc>     m_objDesc;    // Model description for device access
    std::vector<ObjInstance> m_instances;  // Scene model instances


    void loadModel(const std::string& filename, nvmath::mat4f transform = nvmath::mat4f(1));
    void createObjDescriptionBuffer();
    void createTextureImages(const VkCommandBuffer& cmdBuf, const std::vector<std::string>& textures);
    void updateUniformBuffer(const VkCommandBuffer& cmdBuf);

// ──────────────────────────────────────────────────────────── RASTERIZATION ─────

    // Information pushed at each draw call
    PushConstantRaster m_pcRaster {
        {1},                // Identity matrix
        {10.f, 15.f, 8.f},  // light position
        0,                  // instance Id
        100.f,              // light intensity
        0                   // light type
    };

    void updateDescriptorSet();
    void createUniformBuffer();
    void rasterize(const VkCommandBuffer& cmdBuff);


// ───────────────────────────────────────────────────────── GRAPHIC PIPELINE ─────

    VkPipelineLayout            m_pipelineLayout;
    VkPipeline                  m_graphicsPipeline;
    nvvk::DescriptorSetBindings m_descSetLayoutBind;
    VkDescriptorPool            m_descPool;
    VkDescriptorSetLayout       m_descSetLayout;
    VkDescriptorSet             m_descSet;

    nvvk::Buffer m_bGlobals;  // Device-Host of the camera matrices
    nvvk::Buffer m_bObjDesc;  // Device buffer of the OBJ descriptions

    std::vector<nvvk::Texture> m_textures;  // vector of all textures of the scene

    nvvk::ResourceAllocatorDma m_alloc;  // Allocator for buffer, images, acceleration structures
    nvvk::DebugUtil            m_debug;  // Utility to name objects

    void setup(const VkInstance& instance, const VkDevice& device, const VkPhysicalDevice& physicalDevice, uint32_t queueFamily) override;
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void destroyResources();


// ──────────────────────────────────────────────────────────────── RENDERING ─────

    nvvk::DescriptorSetBindings m_postDescSetLayoutBind;
    VkDescriptorPool            m_postDescPool{VK_NULL_HANDLE};
    VkDescriptorSetLayout       m_postDescSetLayout{VK_NULL_HANDLE};
    VkDescriptorSet             m_postDescSet{VK_NULL_HANDLE};
    VkPipeline                  m_postPipeline{VK_NULL_HANDLE};
    VkPipelineLayout            m_postPipelineLayout{VK_NULL_HANDLE};
    VkRenderPass                m_offscreenRenderPass{VK_NULL_HANDLE};
    VkFramebuffer               m_offscreenFramebuffer{VK_NULL_HANDLE};
    nvvk::Texture               m_offscreenColor;
    nvvk::Texture               m_offscreenDepth;
    VkFormat                    m_offscreenColorFormat{VK_FORMAT_R32G32B32A32_SFLOAT};
    VkFormat                    m_offscreenDepthFormat{VK_FORMAT_X8_D24_UNORM_PACK32};


    void createOffscreenRender();
    void createPostPipeline();
    void createPostDescriptor();
    void updatePostDescriptorSet();
    void drawPost(VkCommandBuffer cmdBuf);
    void onResize(int /*w*/, int /*h*/) override;

    void resetFrame();
    void updateFrame();

// ────────────────────────────────────────────────────────────── RAY TRACING ─────

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_rtProperties {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR
    };

    nvvk::RaytracingBuilderKHR m_rtBuilder;

    nvvk::DescriptorSetBindings m_rtDescSetLayoutBind;
    VkDescriptorPool            m_rtDescPool;
    VkDescriptorSetLayout       m_rtDescSetLayout;
    VkDescriptorSet             m_rtDescSet;

    std::vector<VkRayTracingShaderGroupCreateInfoKHR> m_rtShaderGroups;
    VkPipelineLayout                                  m_rtPipelineLayout;
    VkPipeline                                        m_rtPipeline;

    // Miembros para el Shader Binding Table
    nvvk::Buffer m_rtSBTBuffer;
    VkStridedDeviceAddressRegionKHR m_rgenRegion{};
    VkStridedDeviceAddressRegionKHR m_missRegion{};
    VkStridedDeviceAddressRegionKHR m_hitRegion{};
    VkStridedDeviceAddressRegionKHR m_callRegion{};

    // Push constant for ray tracer
    PushConstantRay m_pcRay{
        {},     // clear color
        {},     // light position
        0,      // light intensity
        0,      // light type
        10,     // max depth
        5,      // number of samples
        0,      // actual frame
        30,     // firefly clamp threshold. ¡Valor temporal!
        false,  // russian roulette
        false,  // next event estimation
    };
    int m_maxAcumFrames {20};


    void initRayTracing();

    auto objectToVkGeometryKHR(const ObjModel& model);
    void createBottomLevelAS();
    void createTopLevelAS();
    void createRtShaderBindingTable();

    void createRtPipeline();
    void createRtDescriptorSet();
    void updateRtDescriptorSet();

    void raytrace (const VkCommandBuffer& cmdBuf, const nvmath::vec4f& clear_color);
};
