
# RF2STABLECAM

This is a plugin for rFactor2 that stabilizes your camera, creating a much more natural & comfortable feel when driving. It doesn't add any fancy g-force effects or anything like that. It just makes the camera real *smooooooth*.

## Support

If you like this plugin, please consider checking out my [twitch channel](https://www.twitch.tv/helloimhana) or [youtube channel](https://www.youtube.com/channel/UCm-EKs1L3ujEjdz3OpqvS-A). Thank you!!

## Installation

Please download the latest zip file here: [Latest Download](https://github.com/HanaMcHanaface/RF2STABLECAM/releases/download/v1.0/RF2STABLECAM64.zip).

**IMPORTANT**: Before we install the plugin we have to change some settings in our player config. I wish this wasn't the case, but this is rFactor2 we're talking about. :D

First, find your rFactor2 install location. If installed with steam it will be in `C:\Program Files (x86)\Steam\steamapps\common\rFactor 2`. Now, inside that folder go to `UserData\player`. Here you will find a file named `player.JSON`, open it in notepad. Now you have to find the following lines, and replace the values so they look the same. Using the search tool might be useful!

```json
"Look Roll Angle": 1,
"Look Up/Down Angle": 1,
"Glance Rate": 500,
```

Now we can actually install the plugin. Go back to the rFactor2 directory, but now go to `Bin64\Plugins`. Open up the zip file you downloaded earlier and move the file named `RF2STABLECAM64.dll` into the Plugins directory. Now open rFactor2 (restart if it was already running). After starting, go to settings and in the bottom right there's a plugin section. Make sure that `RF2STABLECAM64.dll` is on. Enable it if not, and restart the game.

**IMPORTANT**: This plugin is not compatible with the TrackIR plugin that is included by default. If TrackIR is enabled then this plugin will have no effect. Unfortunately this means TrackIR equipment cannot be used in combination with this plugin.

Please enjoy!!
