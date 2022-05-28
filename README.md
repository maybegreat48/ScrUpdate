# ScrUpdate

A tool that can update script globals, locals and more across different game versions.<br>

## Updater
The updater is basically just a simple batch program that scans the script bytecode for signatures which will (hopefully) remain the same across different game versions, automating the process of obtaining values from scripts that change every game update.<br>

An example script can found [here](https://github.com/maybegreat48/ScrUpdate/blob/master/script.su). The syntax is fairly simple and the full list of commands can be found [here](https://github.com/maybegreat48/ScrUpdate/blob/master/src/program.cpp).

```pwsh
> .\ScrUpdate.exe -r .\script.su -d .\scripts\ # dir must contain all decrypted/uncompressed script files extracted using CodeWalker/OpenIV
GSBD = 2703660
GSBD_Kicking = 1885262
GSBD_FM_events = 1921039
GlobalPlayerBD = 2689224
GlobalPlayerBD size = 451
...
```

## Sigmaker

ScrUpdate can automatically create patterns of globals and locals for you<br><br>
Make a simple global signature
```pwsh
> .\ScrUpdate.exe -g .\scripts\freemode.ysc -i 1890190
P"5E ? ? ? 34 ? 41 ? 25 ? 7F 1F 56 ? ? 6E" +1 A ="global" +4 B ="global size"
```

Make a global signature with offset
```pwsh
> .\ScrUpdate.exe -g .\scripts\freemode.ysc -i 2689224 -o 207
P"5E ? ? ? 49 ? ? 41 ? 2A 06 56 ? ? 5D ? ? ?" +1 A ="global" +4 W ="global size" +3 B ="offset"
```

Make a local signature
```pwsh
> .\ScrUpdate.exe -g .\scripts\main.ysc -i 80 -l
P"3B ? 06 56 ? ? 5E ? ? ?" +1 B="local"
```
