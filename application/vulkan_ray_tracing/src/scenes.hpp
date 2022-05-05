enum Scene {
    cube_default,
    medieval_building,
    cube_reflective,
    any_hit,
    cornell_box_original,
    cornell_box_saturada,
    cornell_box_mirror,
    cornell_box_vacia,
    cornell_box_vacia_azul_naranja,
    cornell_box_esferas,
    cornell_box_glossy,
    cornell_box_blanca,
    cubes,
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

// ────────────────────────────────────────────────────────────────────────────────

    SceneInfo(Scene scene) {
        // Todas las cajas tienen el mismo posicionamiento, así que lo dejamos como constantes.
        const nvmath::vec3f eye_cornell    = nvmath::vec3f(-0.007, 1.024, 2.752);
        const nvmath::vec3f center_cornell = nvmath::vec3f(0.000, 1.000, 0.000);

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

        case Scene::cubes:
            models.push_back(ModelInfo{
                "media/scenes/cubes.obj"
            });

            break;

    //
    // ──────────────────────────────────────────────────────────── CORNELL BOXES ─────
    //

        case Scene::cornell_box_saturada:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_saturada.obj",
                nvmath::scale_mat4(nvmath::vec3f(0.01f))
            });

            eye    = nvmath::vec3f(2.33665, 3.24268, -5.08527);
            center = nvmath::vec3f(2.32440, 2.97224, 0.11248);

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_original:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_original.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_mirror:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_mirror.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

            break;

        // ─────────────────────────────────────────────────────────────────


        case Scene::cornell_box_vacia:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_empty_rg.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_vacia_azul_naranja:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_empty_co.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_esferas:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_sphere.obj",
            });

            eye    = nvmath::vec3f(0, 0.75, 2.5);
            center = nvmath::vec3f(0, 0.8, 0);

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_glossy:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_glossy.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

            break;

        // ─────────────────────────────────────────────────────────────────

        case Scene::cornell_box_blanca:
            models.push_back(ModelInfo{
                "media/scenes/cornell_box_empty_white.obj",
            });

            eye    = eye_cornell;
            center = center_cornell;

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
