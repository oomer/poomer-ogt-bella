prototype [MagicaVoxel](https://ephtracy.github.io) .vox to DiffuseLogic's [Bella](https://bellarender.com) .bsz


# Build

Download SDK for your OS and move **bella_scene_sdk** into your **workdir**. On Windows rename unzipped folder by removing version ie bella_engine_sdk-24.6.0 -> bella_scene_sdk
https://bellarender.com/builds/

```
workdir/
├── bella_scene_sdk/
├── opengametools/
└── poomer-ogt-bella/
```


# MacOS

```
mkdir workdir
git clone https://github.com/jpaver/opengametools.git
git clone https://github.com/oomer/poomer-ogt-bella.git
cd poomer-ogt-bella
make all -j4
```

