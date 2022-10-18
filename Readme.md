
# RF2STABLECAM

This is a plugin for rFactor2 that stabilizes your cockpit/hood camera, creating a much more natural & comfortable feel when driving. It doesn't add any fancy g-force effects or anything like that. It just makes the camera real *smooooooth*.

## Support

If you like this plugin, please consider checking out my [twitch channel](https://www.twitch.tv/helloimhana) or [youtube channel](https://www.youtube.com/channel/UCm-EKs1L3ujEjdz3OpqvS-A). Thank you!!

## Installation

Download the latest version here: [Latest Download](https://github.com/HanaMcHanaface/RF2STABLECAM/releases/download/V1.3/RF2STABLECAM64.V1.3.zip).

**IMPORTANT**: After installing the plugin you have to start and then close rFactor2 once. The plugin will change your player settings. It might be a good idea to back them up.

This plugin is installed like any other, I will describe it here. First, find your rFactor2 install location and plugin folder. If installed with steam it will be in `C:\Program Files (x86)\Steam\steamapps\common\rFactor 2\Bin64\Plugins`. In the ZIP file you will find a file named `RF2STABLECAM64.dll`, move this file to the Plugins folder.

Now start rFactor2 and go to the settings. In the bottom right you will find the "PLUGINS" section. Make sure that `RF2STABLECAM.dll` is turned on. Now close rFactor2. This is necessary because the plugin has to edit your player settings, which can only be done when closing rFactor2.

If you have other camera plugins they might interfere with this plugin. The plugin is only active when you are driving the car and the camera is a "driving" camera (switch driving camera with the `insert` key by default). Plugins that alter the camera in other views than driving views should be compatible with this plugin.

**IMPORTANT**: This plugin is not compatible with the TrackIR plugin that is included by default. The plugin is automatically disabled when you close rFactor2. This means use of any TrackIR gear is not possible when using this plugin.

Please enjoy!!

## Uninstall

To uninstall simply remove the dll from the Plugins folder. As mentioned, This plugin will alter your player settings. The settings that changed are documented in a file called `STABLECAM_LOG.txt` in your rFactor2 folder.

## Issues

If you have any issues feel free to contact me using any method you prefer. (github, [twitch](https://www.twitch.tv/helloimhana), [twitter](https://twitter.com/HanaMcHanaface) and so on)

## Further Development

If you want to use the source code to write your own plugin, that's fine. But please give credit to me. If you have any questions you can always contact me. The plugin was compiled using Visual Studio 2022 with the solution file `VC11\InternalsPlugin.sln`. Things are named weird because it's based on the "rFactor2 internals plugin" found [here](https://www.studio-397.com/modding-resources/).
