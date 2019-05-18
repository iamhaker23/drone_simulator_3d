#version 150

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;

in  vec3 in_Position;  // Position coming in
in  vec2 in_TexCoord;  // texture coordinate coming in
in  vec3 in_Normal;    // vertex normal used for lighting

uniform vec4 LightPosA;  // light position
uniform vec4 LightPosB;  // light position
uniform vec4 LightPosC;  // light position
uniform vec4 LightPosD;  // light position
uniform float scale;

out vec2 ex_TexCoord;  // exiting texture coord
out vec3 ex_Normal;    // exiting normal transformed by the normal matrix
out vec3 ex_PositionEye; 
out vec3 ex_LightDirA;
out vec3 ex_LightDirB;
out vec3 ex_LightDirC;
out vec3 ex_LightDirD;

void main(void)
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4((in_Position)*scale, 1.0);
	
	ex_TexCoord = in_TexCoord;
		
	ex_Normal = NormalMatrix*in_Normal; 

	ex_PositionEye = vec3((ModelViewMatrix * vec4(in_Position*scale, 1.0)));

	ex_LightDirA = (ViewMatrix * LightPosA).xyz;
    ex_LightDirB = (ViewMatrix * LightPosB).xyz;
    ex_LightDirC = (ViewMatrix * LightPosC).xyz;
    ex_LightDirD = (ViewMatrix * LightPosD).xyz;
    
}