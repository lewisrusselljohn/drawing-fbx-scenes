#pragma once

/* Texture program vertex shader */
const char* vs_source = R"(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 tex_coord;

    out vec2 uv;

    uniform mat4 mv;
    uniform mat4 proj;
    uniform mat4 world;

    out vec3 fragmentColor;
    void main()
    {
        gl_Position = proj*mv*world*vec4(position, 1.0);
        fragmentColor = color;
        uv = tex_coord;
    }
)";

// Texture program fragment shader source
const char* fs_source = R"(
    #version 330 core
    uniform sampler2D texture_sampler;

    in vec3 fragmentColor;
    out vec4 fragColor;
    in vec2 uv;

    void main()
    {
        vec4 texColor = texture(texture_sampler, uv);
        //fragColor = vec4(fragmentColor, 1.0);
        fragColor = texColor;
    }
)";
