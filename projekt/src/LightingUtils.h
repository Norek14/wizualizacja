#include "Shader.h"

class LightingUtils
{
public:
    inline static void SetLightConstants(Shader& shader)
    {
        shader.Bind();
        shader.SetUniform3f("firstPointLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.SetUniform3f("firstPointLight.diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetUniform3f("firstPointLight.specular", 1.0f, 1.0f, 1.0f);
        shader.SetUniform1f("firstPointLight.constant", 1.0f);
        shader.SetUniform1f("firstPointLight.linear", 0.09f);
        shader.SetUniform1f("firstPointLight.quadratic", 0.032f);

        shader.SetUniform3f("secondPointLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.SetUniform3f("secondPointLight.diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetUniform3f("secondPointLight.specular", 1.0f, 1.0f, 1.0f);
        shader.SetUniform1f("secondPointLight.constant", 1.0f);
        shader.SetUniform1f("secondPointLight.linear", 0.09f);
        shader.SetUniform1f("secondPointLight.quadratic", 0.032f);

        shader.SetUniform3f("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.SetUniform3f("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader.SetUniform1f("spotLight.constant", 1.0f);
        shader.SetUniform1f("spotLight.linear", 0.09f);
        shader.SetUniform1f("spotLight.quadratic", 0.032f);

        shader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  
        for (int i = 0; i < 10; i++)
        {
            std::string ambient = "dayLights[" + std::to_string(i) + "].ambient";
            std::string diffuse = "dayLights[" + std::to_string(i) + "].diffuse";
            std::string specular = "dayLights[" + std::to_string(i) + "].specular";
            std::string constant = "dayLights[" + std::to_string(i) + "].constant";
            std::string linear = "dayLights[" + std::to_string(i) + "].linear";
            std::string quadratic = "dayLights[" + std::to_string(i) + "].quadratic";
            shader.SetUniform3f(ambient, 0.05f, 0.05f, 0.05f);
            shader.SetUniform3f(diffuse, 0.3f, 0.3f, 0.3f);
            shader.SetUniform3f(specular, 0.5f, 0.5f, 0.5f);
            shader.SetUniform1f(constant, 1.0f);
            shader.SetUniform1f(linear, 0.09f);
            shader.SetUniform1f(quadratic, 0.032f);
        }
    }
    inline static void DisableDayLights(Shader& shader)
    {
        shader.Bind();
        for (int i = 0; i < 10; i++)
        {
            std::string ambient = "dayLights[" + std::to_string(i) + "].ambient";
            std::string diffuse = "dayLights[" + std::to_string(i) + "].diffuse";
            std::string specular = "dayLights[" + std::to_string(i) + "].specular";
            shader.SetUniform3f(ambient, 0.0f, 0.0f, 0.0f);
            shader.SetUniform3f(diffuse, 0.0f, 0.0f, 0.0f);
            shader.SetUniform3f(specular, 0.0f, 0.0f, 0.0f);
        }
    }
    inline static void EnableDayLights(Shader& shader)
    {
        shader.Bind();
        for (int i = 0; i < 10; i++)
        {
            std::string ambient = "dayLights[" + std::to_string(i) + "].ambient";
            std::string diffuse = "dayLights[" + std::to_string(i) + "].diffuse";
            std::string specular = "dayLights[" + std::to_string(i) + "].specular";
            shader.SetUniform3f(ambient, 0.05f, 0.05f, 0.05f);
            shader.SetUniform3f(diffuse, 0.3f, 0.3f, 0.3f);
            shader.SetUniform3f(specular, 0.5f, 0.5f, 0.5f);
        }
    }
};