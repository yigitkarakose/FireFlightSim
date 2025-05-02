#version 430 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightDir;        
uniform vec3 lightColor;      

uniform vec3 fillDir;         
uniform vec3 fillColor;       

uniform vec3 viewPos;         

uniform vec3 objectColor;      
uniform float shininess;       
uniform float specularStrength;

uniform bool isFirePrism = false;
uniform float fireHeight = 0.0;  

void main() {
    vec3 finalColor = objectColor;
    

    if (isFirePrism) {

        vec3 redColor = vec3(1.0, 0.0, 0.0);
        vec3 yellowColor = vec3(1.0, 1.0, 0.0);
        finalColor = mix(redColor, yellowColor, clamp(fireHeight, 0.0, 1.0));
        
        float flickerIntensity = 1.0 + 0.2 * sin(FragPos.y * 10.0);
        finalColor *= flickerIntensity;
    } else {
   
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * (lightColor + fillColor);

        vec3 N = normalize(Normal);
        vec3 V = normalize(viewPos - FragPos);

        vec3 L1 = normalize(-lightDir);
        float diff1 = max(dot(N, L1), 0.0);
        vec3 diffuse1 = diff1 * lightColor;
        vec3 R1 = reflect(-L1, N);
        float spec1 = pow(max(dot(V, R1), 0.0), shininess);
        vec3 specular1 = specularStrength * spec1 * lightColor;


        vec3 L2 = normalize(-fillDir);
        float diff2 = max(dot(N, L2), 0.0);
        vec3 diffuse2 = diff2 * fillColor;
        vec3 R2 = reflect(-L2, N);
        float spec2 = pow(max(dot(V, R2), 0.0), shininess);
        vec3 specular2 = (specularStrength * 0.5) * spec2 * fillColor; 



        vec3 result = ambient 
                    + diffuse1 + specular1 
                    + diffuse2 + specular2;
        result *= finalColor;


        finalColor = pow(result, vec3(1.0/2.2));
    }

    FragColor = vec4(finalColor, 1.0);
}
