#include "esUtil.h"
#include <stdio.h>

GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   shader = glCreateShader ( type );
   if ( shader == 0 )
   	return 0;

   glShaderSource ( shader, 1, &shaderSrc, NULL );
   glCompileShader ( shader );

   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = new char[infoLen];

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
         
         delete[] infoLog;
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}

GLuint ESUTIL_API esLoadShaderFromFile ( GLenum type, const char *filename)
{
	GLuint shader;

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) 
		return 0;
	
	char c;
	int len = 0;
	while (feof(fp) == 0)
	{
		fread(&c, sizeof(char), 1, fp);
		len++;
	}

	rewind(fp);
	char *str = new char[len];
	fread(str, sizeof(char), len-1, fp);
	str[len-1] = '\0';

	const char *source = (const char*)str;
	shader = esLoadShader(type, source);

	delete[] str;
	fclose(fp);

	return shader;
}

GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   vertexShader = esLoadShader ( GL_VERTEX_SHADER, vertShaderSrc );
   if ( vertexShader == 0 )
      return 0;

   fragmentShader = esLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
   if ( fragmentShader == 0 )
   {
      glDeleteShader( vertexShader );
      return 0;
   }

   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   glLinkProgram ( programObject );

   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog =new char[infoLen];

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );            
         
         delete[] infoLog;
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   glDeleteShader ( vertexShader );
   glDeleteShader ( fragmentShader );

   return programObject;
}

GLuint ESUTIL_API esLoadProgramFromFile ( const char *vertShaderFile, const char *fragShaderFile )
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	vertexShader = esLoadShaderFromFile( GL_VERTEX_SHADER, vertShaderFile );
	if ( vertexShader == 0 )
		return 0;

	fragmentShader = esLoadShaderFromFile ( GL_FRAGMENT_SHADER, fragShaderFile );
	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return 0;
	}

	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	glLinkProgram ( programObject );

	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog =new char[infoLen];

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			esLogMessage ( "Error linking program:\n%s\n", infoLog );            

			delete[] infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	glDeleteShader ( vertexShader );
	glDeleteShader ( fragmentShader );

	return programObject;
}