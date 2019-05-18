#version 150

in  vec2 ex_TexCoord; //texture coord arriving from the vertex
in  vec3 ex_Normal;  //normal arriving from the vertex

out vec4 out_Color;   //colour for the pixel
in vec3 ex_LightDirA;  //light direction arriving from the vertex
in vec3 ex_LightDirB;  //light direction arriving from the vertex
in vec3 ex_LightDirC;  //light direction arriving from the vertex
in vec3 ex_LightDirD;  //light direction arriving from the vertex

in vec3 ex_PositionEye;

uniform int numLights;  // light count
uniform float alphaClip;
uniform vec2 uvOffset;

uniform vec4 light_ambienta;
uniform vec4 light_diffusea;
uniform vec4 light_speculara;

uniform vec4 light_ambientb;
uniform vec4 light_diffuseb;
uniform vec4 light_specularb;

uniform vec4 light_ambientc;
uniform vec4 light_diffusec;
uniform vec4 light_specularc;

uniform vec4 light_ambientd;
uniform vec4 light_diffused;
uniform vec4 light_speculard;

uniform float light_powera;
uniform float light_powerb;
uniform float light_powerc;
uniform float light_powerd;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform int shadeless;
uniform int normalMapping;
uniform float scale;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

void main(void)
{
	//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture values
    //out_Color = vec4(ex_Normal,1.0); //Show normals
	//out_Color = vec4(ex_TexCoord,0.0,1.0); //show texture coords
        
    vec4 texel = texture(DiffuseMap, ex_TexCoord+uvOffset);
    vec4 normalTexel = texture(NormalMap, ex_TexCoord+uvOffset);
    if (alphaClip > 0.0 && texel.a <= alphaClip){
        discard;
    }
    
    if (shadeless == 0){
        //Calculate lighting
        vec3 n, La, Lb, Lc, Ld;
        vec3 ra, rb, rc, rd;
        vec4 color;
        float a_NdotL, b_NdotL, c_NdotL, d_NdotL;
        float a_RdotV, b_RdotV, c_RdotV, d_RdotV;
        
        if (normalMapping > 0){
            n = cross(normalTexel.rgb, ex_Normal);
        }else{
            n = normalize(ex_Normal);
        }
        
        vec3 v = normalize(-ex_PositionEye);
        
        if (numLights > 0){
            La = normalize(ex_LightDirA);
            ra = normalize(-reflect(La, n));
            a_RdotV = max(0.0, dot(ra, v));
            a_NdotL = max(dot(n, La),0.0);   
        }
        if (numLights > 1){
            Lb = normalize(ex_LightDirB);
            rb = normalize(-reflect(Lb, n));
            b_RdotV = max(0.0, dot(rb, v));
            b_NdotL = max(dot(n, Lb),0.0);   
        }
        if (numLights > 2){
            Lc = normalize(ex_LightDirC);
            rc = normalize(-reflect(Lc, n));
            c_RdotV = max(0.0, dot(rc, v));
            c_NdotL = max(dot(n, Lc),0.0);   
        }
        if (numLights > 3){
            Ld = normalize(ex_LightDirD);
            rd = normalize(-reflect(Ld, n));
            d_RdotV = max(0.0, dot(rd, v));
            d_NdotL = max(dot(n, Ld),0.0);   
        }
        
        color = vec4(0,0,0,1);
        
        if (numLights > 0){
            float dist = length(ex_LightDirA-ex_PositionEye);
            float attenuation =(light_powera) / (dist*dist);
            color += light_ambienta * material_diffuse * 0.05f * light_powera;
            color += (light_diffusea * material_diffuse * a_NdotL) * attenuation;
            color += (material_specular * light_speculara) * pow(a_RdotV, material_shininess) * attenuation;
        }
        if (numLights > 1){
            float dist = length(ex_LightDirB-ex_PositionEye);
            float attenuation =(light_powerb) / (dist*dist);
            color += light_ambientb * material_diffuse  * 0.05f * light_powerb;
            color += (light_diffuseb * material_diffuse * b_NdotL)* attenuation;
            color += (material_specular * light_specularb) * pow(b_RdotV, material_shininess)* attenuation;
        }
        if (numLights > 2){
            float dist = length(ex_LightDirC-ex_PositionEye);
            float attenuation =(light_powerc) / (dist*dist);
            color += light_ambientc * material_diffuse * 0.05f * light_powerc;
            color += (light_diffusec * material_diffuse * c_NdotL)* attenuation;
            color += (material_specular * light_specularc) * pow(c_RdotV, material_shininess)* attenuation;
        }
        if (numLights > 3){
            float dist = length(ex_LightDirD-ex_PositionEye);
            float attenuation =(light_powerd) / (dist*dist);
            color += light_ambientd * material_diffuse * 0.05f * light_powerd;
            color += (light_diffused * material_diffuse * d_NdotL)* attenuation;
            color += (material_specular * light_speculard) * pow(d_RdotV, material_shininess)* attenuation;
        }

        //out_Color = vec4(color.rgb, 1.0);  //show just lighting
        out_Color = vec4((color.rgb * texel.rgb), material_diffuse.a*texel.a); //texture and lighting
    }else{
        out_Color = vec4(texel.rgb*material_diffuse.rgb, min(material_diffuse.a, texel.a)); //texture and lighting
    }
    
}