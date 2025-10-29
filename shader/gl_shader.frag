
#version 330 core
in vec4 f_worldPos;
in vec3 f_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    

    // Ambient
    float ambient_p = 0.1f;
    vec3 ambient = ambient_p * lightColor;

    // Diffuse
    //Calculate the amount of light hitting each surface

    vec3 f_norm = normalize(f_normal);
    vec3 lightFaceDir = normalize(lightPos - vec3(f_worldPos));
    
    //Calculate the degree of coincidence between surfaces and lights
    // dot Matching of surface and light
    // max When facing backwards, set the light to 0
    float diffPer = max(dot(f_norm, lightFaceDir), 0.0);
    vec3 diffuse = diffPer * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewFaceDir = normalize(viewPos - vec3(f_worldPos));
    vec3 incidentLDir = -lightFaceDir; 
    //Calculate the reflection direction
    vec3 reflectDir = reflect(incidentLDir, f_norm);
    int materialShiness = 32;
    //Reflection values are expressed as coefficients
    float specPer = pow(max(dot(viewFaceDir, reflectDir), 0.0), materialShiness);
    vec3 specular = specularStrength * specPer * lightColor;

    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(resultColor, 1.0);
    
    /*
    float ambient_p = 0.1f;
    vec3 ambient = ambient_p * lightColor;

    vec3 n_normalized = normalize(f_normal);
    vec3 lightFaceDir = normalize(lightPos - vec3(f_worldPos));
    float diffPer = max(dot(n_normalized,lightFaceDir),0.0);
    float gradedIntensity;
    if(diffPer > 0.7)gradedIntensity = 1.0;
    else if(diffPer > 0.5)gradedIntensity = 0.7;
    else if(diffPer > 0.25) gradedIntensity = 0.4;
    else gradedIntensity = 0.1;
    vec3 resultColor = ambient + (gradedIntensity * objectColor);
    FragColor = vec4(resultColor, 1.0);
    */
}
