/*
Visualize tile buffer for debug purposes.
*/
#version 400

uniform usampler2D tLightTiles;

uniform uvec2 screenPos;

const uint tileSize = 8;
const uint maxTileLights = 16;

out vec4 fragmentColor;

void main () {
    // Figure out which tile we're in.
    uint hTile = uint(gl_FragCoord.x) - screenPos.x;
    uint vTile = uint(gl_FragCoord.y) - screenPos.y;
    
    // Count lights in the tile.
    uint lightCount = 0;
    for (uint i=0; i < maxTileLights; ++i) {
        uint lightIndex = texelFetch(tLightTiles, ivec2(hTile, vTile * maxTileLights + i), 0).r;
        if (lightIndex == 0)
            break;
        
        ++lightCount;
    }
    
    //lightCount = texelFetch(tLightTiles, ivec2(hTile, vTile * maxTileLights + 0), 0).r;
    
    if (lightCount == maxTileLights) {
        fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        float ln = float(lightCount) / float(maxTileLights);
        fragmentColor = vec4(ln, ln, ln, 1.0);
    }
}
