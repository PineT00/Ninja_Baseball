// magenta_to_transparent.frag
#ifdef GL_ES
precision mediump float;
#endif

// 텍스처 유닛 0에서 텍스처 샘플러
uniform sampler2D texture;

// 정점 셰이더에서 전달된 텍스처 좌표
varying vec2 TexCoords;

void main()
{
    // 현재 픽셀의 색상을 텍스처에서 가져옴
    vec4 pixelColor = texture2D(texture, gl_TexCoord[0].xy);
    
    // 마젠타 색상과의 차이를 계산
    float magenta_diff = distance(pixelColor.rgb, vec3(1.0, 0.0, 1.0)); // RGB에서 마젠타는 (1.0, 0.0, 1.0)

    // 녹색 색상과의 차이를 계산
    float green_diff = distance(pixelColor.rgb, vec3(0.0, 1.0, 0.0)); // RGB에서 녹색은 (0.0, 1.0, 0.0)

    // 마젠타 또는 녹색과의 차이가 매우 작으면(즉, 거의 같은 색상이면) 알파 값을 0으로 설정
    if(magenta_diff < 0.6 || green_diff < 0.6) // 색상 차이 임계값은 상황에 따라 조절할 수 있음
    {
        gl_FragColor = vec4(pixelColor.rgb, 0.0);
    }
    else
    {
        gl_FragColor = pixelColor;
    }
}
