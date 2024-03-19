#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 TexCoords;

void main()
{
    // 현재 픽셀의 색상을 텍스처에서 가져옴
    vec4 pixelColor = texture2D(texture, TexCoords);

    // 특정 색상 (여기서는 RGB(2, 128, 127))과의 차이 계산
    float colorDiff = distance(pixelColor.rgb, vec3(2.0 / 255.0, 128.0 / 255.0, 127.0 / 255.0));

    // 색상 차이가 매우 작으면 (즉, 거의 같은 색상이면) 알파 값을 0으로 설정
    // 차이 임계값은 상황에 따라 조절할 수 있음, 여기서는 예제로 0.1을 사용
    if(colorDiff < 0.1)
    {
        gl_FragColor = vec4(pixelColor.rgb, 0.0);
    }
    else
    {
        gl_FragColor = pixelColor;
    }
}
