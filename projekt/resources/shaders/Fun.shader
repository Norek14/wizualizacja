#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // varying

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.5);
	// 0.2 - waga mixu
	//vec4 texColor = texture(u_Texture1, v_TexCoord);
	//color = texColor;
}
