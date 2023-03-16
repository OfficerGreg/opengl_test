#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    //FragColor = vec4(1.0f, 0.2f, 0.6f, 1.0f);
    //FragColor = vec4(ourColor, 1.0);
    //FragColor = vec4(ourColor, 1.0f) * texture(texture1, texCoord);
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
}