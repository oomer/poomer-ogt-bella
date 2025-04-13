
#include <iostream> // For input/output operations (cout, cin, etc.)

// Bella SDK includes - external libraries for 3D rendering
#include "../bella_scene_sdk/src/bella_sdk/bella_scene.h" // For creating and manipulating 3D scenes in Bella
//#include "../bella_scene_sdk/src/dl_core/dl_main.inl" // Core functionality from the Diffuse Logic engine

#ifdef _WIN32
#include <windows.h>    // For ShellExecuteW
#include <shellapi.h>   // For ShellExecuteW
#include <codecvt>      // For wstring_convert
#endif

#include "oomer_bella_scene.h"   // common bella scene code
#include "oomer_misc.h"         // common misc code

#define OGT_VOX_IMPLEMENTATION
#include "../opengametools/src/ogt_vox.h"

dl::bella_sdk::Node essentialsToScene(dl::bella_sdk::Scene& belScene);


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
        std::cout << printLicense() << std::endl;
        return 0;
    }
 
    // If --thirdparty was requested, print third-party licenses and exit
    if (args.have("--thirdparty"))
    {
        std::cout << printBellaSDKThirdPartyLicence() << "\n===\n" << std::endl;
        std::cout << printOpenGameToolsThirdPartyLicence() << "\n===\n" << std::endl;
        return 0;
    }

    if (args.have("--input"))
    {
        dl::String bszName;
        dl::String objName;
        dl::String vmaxDirName;
        vmaxDirName = args.value("--input");
        bszName = vmaxDirName.replace("vmax", "bsz");
        objName = vmaxDirName.replace("vmax", "obj");

        // Create a new scene
        dl::bella_sdk::Scene belScene;
        belScene.loadDefs();
        auto belWorld = essentialsToScene(belScene);
        auto belMeshXform   = belScene.createNode("xform", "oomerMeshXform");
        auto belMeshVoxel   = belScene.createNode("mesh", "oomerMeshVoxel");
        belMeshVoxel.parentTo(belMeshXform);
        belMeshXform.parentTo(belWorld);
        addMeshCube(belMeshVoxel);

        // Save the scene to a Bella scene file
        belScene.write("foo.bsz");
    }
    
    return 0;
}