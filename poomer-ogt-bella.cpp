
#include <iostream> // For input/output operations (cout, cin, etc.)

// Bella SDK includes - external libraries for 3D rendering
#include "../bella_scene_sdk/src/bella_sdk/bella_scene.h" // For creating and manipulating 3D scenes in Bella
#include "../bella_scene_sdk/src/dl_core/dl_main.inl" // Core functionality from the Diffuse Logic engine

#ifdef _WIN32
#include <windows.h>    // For ShellExecuteW
#include <shellapi.h>   // For ShellExecuteW
#include <codecvt>      // For wstring_convert
#endif

#include "../oom/oom_bella_long.h"        
#include "../oom/oom_bella_scene.h"        
#include "../oom/oom_license.h"        

#define OGT_VOX_IMPLEMENTATION
#include "../opengametools/src/ogt_vox.h"

dl::bella_sdk::Node oom::bella::essentialsToScene(dl::bella_sdk::Scene& belScene);
const ogt_vox_scene* load_vox_scene(const char* filename, uint32_t scene_read_flags);

int DL_main(dl::Args& args) {
    args.add("i", "input", "", "vmax directory or vmax.zip file");
    args.add("mo", "mode", "", "mode for output, mesh, voxel, or both");
    args.add("mt", "meshtype", "", "meshtype classic, greedy, other");
    args.add("be", "bevel", "", "add bevel to material");
    args.add("tp",  "thirdparty",   "",   "prints third party licenses");
    args.add("li",  "licenseinfo",   "",   "prints license info");

    // If --help was requested, print help and exit
    if (args.helpRequested()) {
        std::cout << args.help("poomer-ogt-bella © 2025 Harvey Fong","poomer-ogt-bella", "1.0") << std::endl;
        return 0;
    }
    
    // If --licenseinfo was requested, print license info and exit
    if (args.have("--licenseinfo"))
    {
        std::cout << "poomer-ogt-bella\n\nCopyright 2025 Harvey Fong" << std::endl;
        std::cout << oom::license::printLicense() << std::endl;
        return 0;
    }
 
    // If --thirdparty was requested, print third-party licenses and exit
    if (args.have("--thirdparty"))
    {
        std::cout << oom::license::printBellaSDK() << "\n===\n" << std::endl;
        std::cout << oom::license::printOpenGameTools() << "\n===\n" << std::endl;
        return 0;
    }

    if (args.have("--input"))
    {
        dl::String bszName;
        dl::String voxName;
        voxName = args.value("--input");
        bszName = voxName.replace("vox", "bsz");

        // Create a new scene
        dl::bella_sdk::Scene belScene;
        belScene.loadDefs();
        auto belWorld = oom::bella::essentialsToScene(belScene);

        auto voxScene = load_vox_scene(voxName.buf(), 0);

        if (!voxScene)
        {
            std::cerr << "Failed to load voxel scene" << std::endl;
            return 1;
        }
        for (uint32_t model_index = 0; model_index < voxScene->num_models; model_index++) {
            std::cout << "Model " << model_index << std::endl;
            const ogt_vox_model* model = voxScene->models[model_index];
            // find the model name by finding a named instance that references it.
            const char* model_name = NULL;
            for (uint32_t instance_index = 0; instance_index < voxScene->num_instances; instance_index++) {
                if (voxScene->instances[instance_index].name && voxScene->instances[instance_index].model_index == model_index) {
                    if (!model_name) {
                        model_name = voxScene->instances[instance_index].name;
                    }
                }
            }
            if (model_name) {
                std::cout << "Model name: " << model_name << std::endl;
            }
        }

        belScene.write("foo.bsz");



    }
    
    return 0;
}


// a helper function to load a magica voxel scene given a filename.
const ogt_vox_scene* load_vox_scene(const char* filename, uint32_t scene_read_flags)
{
    // open the file
    FILE * fp = fopen(filename, "rb");
    if (!fp)
        return NULL;

    // get the buffer size which matches the size of the file
    fseek(fp, 0, SEEK_END);
    uint32_t buffer_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // load the file into a memory buffer
    uint8_t * buffer = new uint8_t[buffer_size];
    fread(buffer, buffer_size, 1, fp);
    fclose(fp);

    // construct the scene from the buffer
    const ogt_vox_scene * scene = ogt_vox_read_scene_with_flags(buffer, buffer_size, scene_read_flags);

    // the buffer can be safely deleted once the scene is instantiated.
    delete[] buffer;

    return scene;
}