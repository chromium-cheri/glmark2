#include "shader.h"

char *readShaderFile(const char *FileName)
{
    FILE *fp;
    char *DATA = NULL;

    int flength = 0;

    fp = fopen(FileName,"rt");

    fseek(fp, 0, SEEK_END);

    flength = ftell(fp);

    rewind(fp);


    DATA = (char *)malloc(sizeof(char) * (flength+1));
    flength = fread(DATA, sizeof(char), flength, fp);
    DATA[flength] = '\0';

    fclose(fp);

    return DATA;
}

Shader::~Shader()
{
    unload();
}

void Shader::load(const char *pVertexShaderFileName, const char *pFragmentShaderFileName)
{
    char *vertex_shader_source, *fragment_shader_source;
    char msg[512];

    mVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);
    mFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    vertex_shader_source = readShaderFile(pVertexShaderFileName);

    fragment_shader_source = readShaderFile(pFragmentShaderFileName);

    const char *vs = vertex_shader_source;
    const char *fs = fragment_shader_source;

    glShaderSourceARB(mVertexShader, 1, &vs, NULL);
    glShaderSourceARB(mFragmentShader, 1, &fs, NULL);

    free(vertex_shader_source);
    free(fragment_shader_source);

    glCompileShaderARB(mVertexShader);
    glGetShaderInfoLog(mVertexShader, sizeof msg, NULL, msg);
    if (strlen(msg) > 0)
        printf("%s: %s", pVertexShaderFileName, msg);

    glCompileShaderARB(mFragmentShader);
    glGetShaderInfoLog(mFragmentShader, sizeof msg, NULL, msg);
    if (strlen(msg) > 0)
        printf("%s: %s\n", pFragmentShaderFileName, msg);

    mShaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(mShaderProgram, mFragmentShader);
    glAttachObjectARB(mShaderProgram, mVertexShader);
    glBindAttribLocation(mShaderProgram, VertexAttribLocation, "position");
    glBindAttribLocation(mShaderProgram, NormalAttribLocation, "normal");
    glBindAttribLocation(mShaderProgram, TexCoordAttribLocation, "texcoord");

    glLinkProgram(mShaderProgram);
    glGetShaderInfoLog(mShaderProgram, sizeof msg, NULL, msg);
    if (strlen(msg) > 0)
        printf("Shader Linking: %s\n", msg);

    mLocations.ModelViewProjectionMatrix = glGetUniformLocation(mShaderProgram,
            "ModelViewProjectionMatrix");
    mLocations.NormalMatrix = glGetUniformLocation(mShaderProgram,
            "NormalMatrix");
    mLocations.LightSourcePosition = glGetUniformLocation(mShaderProgram,
            "LightSourcePosition");
    mLocations.LightSourceHalfVector = glGetUniformLocation(mShaderProgram,
            "LightSourceHalfVector");

    mLocations.LightSourceAmbient = glGetUniformLocation(mShaderProgram,
            "LightSourceAmbient");
    mLocations.LightSourceDiffuse = glGetUniformLocation(mShaderProgram,
            "LightSourceDiffuse");
    mLocations.LightSourceSpecular = glGetUniformLocation(mShaderProgram,
            "LightSourceSpecular");

    mLocations.MaterialAmbient = glGetUniformLocation(mShaderProgram,
            "MaterialAmbient");
    mLocations.MaterialDiffuse = glGetUniformLocation(mShaderProgram,
            "MaterialDiffuse");
    mLocations.MaterialSpecular = glGetUniformLocation(mShaderProgram,
            "MaterialSpecular");
    mLocations.MaterialColor = glGetUniformLocation(mShaderProgram,
            "MaterialColor");
    mLocations.MaterialTexture0 = glGetUniformLocation(mShaderProgram,
            "MaterialTexture0");
    glUniform1i(mLocations.MaterialTexture0, 0);

#ifdef _DEBUG
    printf("Uniform Locations: %d %d %d %d %d %d %d %d %d %d %d %d\n",
            mLocations.ModelViewProjectionMatrix,
            mLocations.NormalMatrix,
            mLocations.LightSourcePosition,
            mLocations.LightSourceHalfVector,
            mLocations.LightSourceAmbient,
            mLocations.LightSourceDiffuse,
            mLocations.LightSourceSpecular,
            mLocations.MaterialAmbient,
            mLocations.MaterialDiffuse,
            mLocations.MaterialSpecular,
            mLocations.MaterialColor,
            mLocations.MaterialTexture);
#endif

}

void Shader::use()
{
    glUseProgramObjectARB(mShaderProgram);
}

void Shader::remove()
{
    glUseProgramObjectARB(0);
}

void Shader::unload()
{
    glDetachObjectARB(mShaderProgram, mVertexShader);
    glDetachObjectARB(mShaderProgram, mFragmentShader);
    
    glDeleteObjectARB(mVertexShader);
    glDeleteObjectARB(mFragmentShader);
    glDeleteObjectARB(mShaderProgram);

    mVertexShader = 0;
    mFragmentShader = 0;
    mShaderProgram = 0;
}
