#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

struct Material
{
	sampler2D _texture;
	float shininess;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
	float outerCutOff;
};

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 CalcColor;
	flat vec3 FlatCalcColor;
} vs_out;

uniform mat4 u_MVP;

uniform Material material;

uniform PointLight firstPointLight;
uniform PointLight secondPointLight;
uniform PointLight dayLights[10];
uniform SpotLight spotLight;

uniform bool usePhong;
uniform bool useGourand;
uniform bool useFlat;

uniform vec3 viewPos;

float CalculateAttenuation(vec3 position, float constant, float linear, float quadratic)
{
	float distance = length(position - vs_out.FragPos);
	return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

vec3 GetBlinnShading(vec3 position, vec3 _ambient, vec3 _diffuse, vec3 _specular,
	float constant, float linear, float quadratic, vec3 texColor, vec3 normal, vec3 viewDir)
{
	// attenuation
	float attenuation = CalculateAttenuation(position, constant, linear, quadratic);

	// ambient
	vec3 ambient = _ambient * texColor;

	// diffuse
	vec3 lightDir = normalize(position - vs_out.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = _diffuse * diff * texColor;

	// specular blinn
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 specular = _specular * spec * texColor;
	return (ambient + diffuse + specular) * attenuation;
}

vec3 GetBlinnShading(PointLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	return GetBlinnShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);
}

vec3 GetBlinnShading(SpotLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	vec3 result = GetBlinnShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);

	vec3 lightDir = normalize(light.position - vs_out.FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return result * intensity;
}

vec3 GetPhongShading(vec3 position, vec3 _ambient, vec3 _diffuse, vec3 _specular,
	float constant, float linear, float quadratic, vec3 texColor, vec3 normal, vec3 viewDir)
{
	// attenuation
	float attenuation = CalculateAttenuation(position, constant, linear, quadratic);

	// ambient
	vec3 ambient = _ambient * texColor;

	// diffuse
	vec3 lightDir = normalize(position - vs_out.FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = _diffuse * diff * texColor;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = _specular * spec * texColor;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 GetPhongShading(SpotLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	vec3 result = GetPhongShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);

	vec3 lightDir = normalize(light.position - vs_out.FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return result * intensity;
}

vec3 GetPhongShading(PointLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	return GetPhongShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);
}

void main()
{
	gl_Position = u_MVP * position;
	vs_out.FragPos = vec3(position);
	vs_out.Normal = aNormal;
	vs_out.TexCoords = aTexCoords;
	if (useGourand || useFlat)
	{
		vec3 texColor = vec3(1.0);
		vec3 normal = vs_out.Normal;
		vec3 viewDir = normalize(viewPos - vs_out.FragPos);
		if (usePhong)
		{
			if (useGourand)
			{
				vs_out.CalcColor = GetPhongShading(firstPointLight, texColor, normal, viewDir) +
					GetPhongShading(secondPointLight, texColor, normal, viewDir) +
					GetPhongShading(spotLight, texColor, normal, viewDir);
				for (int i = 0; i < 10; i++)
				{
					vs_out.CalcColor += GetPhongShading(dayLights[i], texColor, normal, viewDir);
				}
			}
			if (useFlat)
			{
				vs_out.FlatCalcColor = GetPhongShading(firstPointLight, texColor, normal, viewDir) +
					GetPhongShading(secondPointLight, texColor, normal, viewDir) +
					GetPhongShading(spotLight, texColor, normal, viewDir);
				for (int i = 0; i < 10; i++)
				{
					vs_out.FlatCalcColor += GetPhongShading(dayLights[i], texColor, normal, viewDir);
				}
			}
		}
		else
		{
			if (useGourand)
			{
				vs_out.CalcColor = GetBlinnShading(firstPointLight, texColor, normal, viewDir) +
					GetBlinnShading(secondPointLight, texColor, normal, viewDir) +
					GetBlinnShading(spotLight, texColor, normal, viewDir);
				for (int i = 0; i < 10; i++)
				{
					vs_out.CalcColor += GetBlinnShading(dayLights[i], texColor, normal, viewDir);
				}
			}
			if (useFlat)
			{
				vs_out.FlatCalcColor = GetBlinnShading(firstPointLight, texColor, normal, viewDir) +
					GetBlinnShading(secondPointLight, texColor, normal, viewDir) +
					GetBlinnShading(spotLight, texColor, normal, viewDir);
				for (int i = 0; i < 10; i++)
				{
					vs_out.FlatCalcColor += GetBlinnShading(dayLights[i], texColor, normal, viewDir);
				}
			}
		}
	}
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 CalcColor;
	flat vec3 FlatCalcColor;
} fs_in;


struct Material
{
	sampler2D _texture;
	//vec3 specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
	float outerCutOff;
};

struct Fog
{
	float density;
	vec3 color;
};

uniform Material material;

uniform PointLight firstPointLight;
uniform PointLight secondPointLight;
uniform PointLight dayLights[10];
uniform SpotLight spotLight;
uniform bool useFog;
uniform Fog fog;
uniform bool usePhong;
uniform bool useGourand;
uniform bool useFlat;

uniform vec3 viewPos;

float CalculateAttenuation(vec3 position, float constant, float linear, float quadratic)
{
	float distance = length(position - fs_in.FragPos);
	return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

vec3 GetPhongShading(vec3 position, vec3 _ambient, vec3 _diffuse, vec3 _specular,
	float constant, float linear, float quadratic, vec3 texColor, vec3 normal, vec3 viewDir)
{
	// attenuation
	float attenuation = CalculateAttenuation(position, constant, linear, quadratic);

	// ambient
	vec3 ambient = _ambient * texColor;

	// diffuse
	vec3 lightDir = normalize(position - fs_in.FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = _diffuse * diff * texColor;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = _specular * spec * texColor;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 GetPhongShading(SpotLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	vec3 result = GetPhongShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);

	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return result * intensity;
}

vec3 GetPhongShading(PointLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	return GetPhongShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);
}

vec3 GetBlinnShading(vec3 position, vec3 _ambient, vec3 _diffuse, vec3 _specular,
	float constant, float linear, float quadratic, vec3 texColor, vec3 normal, vec3 viewDir)
{
	// attenuation
	float attenuation = CalculateAttenuation(position, constant, linear, quadratic);

	// ambient
	vec3 ambient = _ambient * texColor;

	// diffuse
	vec3 lightDir = normalize(position - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = _diffuse * diff * texColor;

	// specular blinn
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 specular = _specular * spec * texColor;
	return (ambient + diffuse + specular) * attenuation;
}

vec3 GetBlinnShading(PointLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	return GetBlinnShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);
}

vec3 GetBlinnShading(SpotLight light, vec3 texColor, vec3 normal, vec3 viewDir)
{
	vec3 result = GetBlinnShading(light.position, light.ambient, light.diffuse, light.specular,
		light.constant, light.linear, light.quadratic, texColor, normal, viewDir);

	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return result * intensity;
}

void main()
{
	vec3 result = vec3(0.0);
	vec3 texColor = texture(material._texture, fs_in.TexCoords).rgb;
	if (useGourand)
	{
		result = vec3(texColor * fs_in.CalcColor);
	}
	else if (useFlat)
	{
		result = vec3(texColor * fs_in.FlatCalcColor);
	}
	else
	{
		vec3 normal = normalize(fs_in.Normal);
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);

		if (usePhong)
		{
			result = GetPhongShading(firstPointLight, texColor, normal, viewDir) +
				GetPhongShading(secondPointLight, texColor, normal, viewDir) +
				GetPhongShading(spotLight, texColor, normal, viewDir);
			for (int i = 0; i < 10; i++)
			{
				result += GetPhongShading(dayLights[i], texColor, normal, viewDir);
			}
		}
		else
		{
			result = GetBlinnShading(firstPointLight, texColor, normal, viewDir) +
				GetBlinnShading(secondPointLight, texColor, normal, viewDir) +
				GetBlinnShading(spotLight, texColor, normal, viewDir);
			for (int i = 0; i < 10; i++)
			{
				result += GetBlinnShading(dayLights[i], texColor, normal, viewDir);
			}
		}
	}	
	if (useFog)
	{
		float factor = length(viewPos - fs_in.FragPos) * fog.density;
		float alpha = 1 / exp(factor * factor);
		result = mix(fog.color, result, alpha);
	}
	color = vec4(result, 1.0);
}