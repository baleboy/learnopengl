#version 330 core

out vec4 FragColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
	// FragColor = mix(texture(texture1, ourTexCoord), texture(texture2, ourTexCoord), 0.2);
}