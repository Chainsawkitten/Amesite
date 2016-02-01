/*
Lighting pass fragment shader (second pass)
*/
#version 400

uniform sampler2D tShadowMap;
uniform sampler2D tDiffuse;
uniform sampler2D tNormals;
uniform sampler2D tSpecular;
uniform sampler2D tDepth;

uniform mat4 UVtransformMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 inverseProjectionMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;
uniform vec3 diffuseCoefficient;

uniform float scale;

in vec2 texCoords;

out vec4 fragmentColor;

const vec2 poisson[15] = vec2[](
	vec2( 0.3717325, 0.1279892),
	vec2(0.4879754, -0.3658713),
	vec2(0.794171, 0.3522189),
	vec2(0.3074657, 0.6046221),
	vec2(-0.1719546, 0.1717038),
	vec2(0.8643684, -0.07669879),
	vec2(-0.3394284, -0.2177678),
	vec2(0.07109351, -0.4859956),
	vec2(0.4489738, -0.7939949),
	vec2(-0.1399191, -0.8488325),
	vec2(-0.6998154, -0.5012075),
	vec2(-0.08893247, 0.9369829),
	vec2(-0.4376774, 0.5030558),
	vec2(-0.9372669, 0.04219909),
	vec2(-0.8517231, 0.4333114)
);

// Calculate shadow
float calculateShadow(vec4 lightSpacePosition) {
    vec4 shadowCoord = UVtransformMatrix * lightSpacePosition;

	float visibility = 1.0;
	for (int i = 0; i < 10; i++){
		if (texture(tShadowMap, (shadowCoord.xy / shadowCoord.w) + poisson[i] / 300.0).z < (shadowCoord.z)/ shadowCoord.w){
			visibility -= 1.0/20.0;
		 }
	}
    return visibility;
}

// Divide a color into discrete steps.
vec3 discretify(vec3 color) {
	float steps = 3.0;
	return round(steps * color) / steps;
}

// Ambient, diffuse and specular lighting.
vec3 ads(vec3 diffuse, vec3 normal, vec3 position, vec3 specular) {
	vec4 lightSpacePos = lightProjectionMatrix * lightViewMatrix * 	inverseViewMatrix * vec4(position, 1.0);
	vec3 lightDirection = normalize(vec3(lightPosition) - position);	
	float visibility = calculateShadow(lightSpacePos);
	vec3 v = normalize(vec3(-position));
	vec3 r = normalize(reflect(-lightDirection, normal)) * (visibility * 2.0 - 1.0);
	vec3 diffuseLight = diffuseCoefficient * max(dot(lightDirection, normal), 0.0);
	float shinyPower = 2000.0;
	vec3 Ka = vec3(0.2, 0.2, 0.2);
	vec3 specularLight = specular * pow(max(dot(r, v), 0.0), shinyPower);
	return lightIntensity * (diffuse * discretify(Ka + visibility * diffuseLight) + visibility * specularLight);
}

// Reconstruct position.
vec3 reconstructPos(vec2 texCoord, float depth){
    vec4 sPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    sPos = inverseProjectionMatrix * sPos;

    return (sPos.xyz / sPos.w);
}

void main () {
	float depth = texture(tDepth, texCoords * scale).r;
	vec3 position = reconstructPos(texCoords, depth);
	vec3 diffuse = texture(tDiffuse, texCoords * scale).rgb;
	vec3 normal = texture(tNormals, texCoords * scale).xyz;
	vec3 specular = texture(tSpecular, texCoords * scale).xyz;
	
	fragmentColor = vec4(ads(diffuse, normal, position, specular), 1.0);
	gl_FragDepth = depth;
}