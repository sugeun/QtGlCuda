//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;

//void main(void)
//{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
//}


varying highp vec2 coords;

void main()
{

    gl_FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
