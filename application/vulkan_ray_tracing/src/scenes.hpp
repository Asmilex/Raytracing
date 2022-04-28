enum Scene {
    cube_default,
    medieval_building,
    cube_reflective,
    any_hit,
    cornell_box,
    cornell_box_multimaterial,
};

class ModelInfo {
public:
    std::string obj_path;
    nvmath::mat4f transform;

    ModelInfo(std::string obj_path, std::vector<nvmath::mat4f> transforms) {
        this->obj_path = obj_path;

        this->transform.identity();

        for (const auto m: transforms) {
            this->transform = this->transform * m;
        }
    }

    ModelInfo(std::string obj_path, nvmath::mat4f transform) {
        this->obj_path = obj_path;
        this->transform = transform;
    }

    ModelInfo(std::string obj_path) {
        this->obj_path = obj_path;
        this->transform.identity();
    }
};

class SceneInfo {
public:
    std::vector<ModelInfo> models;

    nvmath::vec3f eye    = nvmath::vec3f(4.0f, 4.0f, 4.0f);
    nvmath::vec3f center = nvmath::vec3f(0, 1, 0);
    nvmath::vec3f up     = nvmath::vec3f(0, 1, 0);


    SceneInfo(Scene scene) {
        switch (scene) {
        case Scene::cube_default:
            models.push_back(ModelInfo {
                "media/scenes/cube_multi.obj",
            });

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::medieval_building:
            models.push_back(ModelInfo {
                "media/scenes/Medieval_building.obj",
            });
            models.push_back(ModelInfo {
                "media/scenes/plane.obj"
            });

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cube_reflective:
            models.push_back(ModelInfo{
                "media/scenes/cube_no_diffuse.obj",
                nvmath::translation_mat4(nvmath::vec3f(-2, 0, 0)) * nvmath::scale_mat4(nvmath::vec3f(.1f, 5.f, 5.f))
            });
            models.push_back(ModelInfo{
                "media/scenes/cube_no_diffuse.obj",
                nvmath::translation_mat4(nvmath::vec3f(2, 0, 0)) * nvmath::scale_mat4(nvmath::vec3f(.1f, 5.f, 5.f))
            });

            models.push_back(ModelInfo{
                "media/scenes/cube_multi.obj"
            });
            models.push_back(ModelInfo{
                "media/scenes/plane.obj",
                nvmath::translation_mat4(nvmath::vec3f(0, -1, 0))
            });

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::any_hit:
            models.push_back(ModelInfo{
                "media/scenes/wuson_transparent.obj"
            });
            models.push_back(ModelInfo{
                "media/scenes/sphere.obj",
                nvmath::scale_mat4(nvmath::vec3f(1.5f)) * nvmath::translation_mat4(nvmath::vec3f(0.0f, 1.0f, 0.0f))
            });
            models.push_back(ModelInfo{
                "media/scenes/plane.obj"
            });

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box.obj",
            });

            eye    = nvmath::vec3f(233.665, 324.268, -508.527);
            center = nvmath::vec3f(232.440, 297.224, 11.248);
            up     = nvmath::vec3f(0, 1, 0);

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_multimaterial:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_multimaterial.obj"
            });

            eye    = nvmath::vec3f(233.665, 324.268, -508.527);
            center = nvmath::vec3f(232.440, 297.224, 11.248);
            up     = nvmath::vec3f(0, 1, 0);

            break;
        }
    }
};


// ────────────────────────────────────────────────────────────────────────────────

extern std::vector<std::string> defaultSearchPaths;

void load_scene(Scene scene, Engine& engine) {
    SceneInfo scene_info(scene);

    for (auto& model : scene_info.models) {
        engine.loadModel (
            nvh::findFile(model.obj_path, defaultSearchPaths, true), model.transform
        );
    }

    CameraManip.setLookat(
        scene_info.eye,
        scene_info.center,
        scene_info.up
    );
}
