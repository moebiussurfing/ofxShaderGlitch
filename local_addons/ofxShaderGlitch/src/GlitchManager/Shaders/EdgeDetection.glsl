
// ==== Canny Edge ==== //
#define PI      3.14159265358979323
#define PI_2    1.57079632679
#define PI_4    0.78539816339
#define PI_8    0.39269908169


#define PI_7_8  2.7488936
#define PI_3_8  1.178097
#define PI_5_8  1.9634954

float highThreshold = .35;
float lowThreshold = .1;

float GREYCol(vec3 rgb){
    return .3086 * rgb.r + .6094 * rgb.g + 0.0820 * rgb.b;
    
}

float GREYTex(sampler2DRect tex, vec2 uv){
//    vec3 rgb = Gaussian5_5(tex, uv).rgb;
    vec3 rgb = texture2DRect(tex, uv).rgb;
    return GREYCol(rgb);
    
}

vec2  offsetTheta(float val){
    
    vec2 dir = vec2(0.);
    if(val == 0.)dir = vec2(0., 1.);
    if(val == 1.)dir = vec2(1., 0.);
    if(val == 2.)dir = vec2(1., -1.);
    if(val == 3.)dir = vec2(1., 1.);
    
    return dir;
}

vec2 convertAngle(float theta){
    
    vec2 offset = vec2(0.);
    // direction vertical neighboor
    if( ( theta >= PI_3_8 && theta < PI_5_8 ) || (theta >= -PI_5_8 && theta < - PI_3_8) )offset = vec2(0., 1.);
//    // direction horizontal neighboor
    if( ( theta >= -PI_8 && theta < PI_8 )  || (theta >= PI_7_8 && theta < PI_7_8 + PI_4) )offset = vec2(1., 0.);
    //first diagonal //
    if( (theta >= PI_8 && theta < PI_3_8 )  || (theta >= -PI_7_8 && theta < -PI_5_8) )offset = vec2(-1., -1.);
    //second diagonal \\
    if( ( theta >= -PI_3_8 && theta < -PI_8 ) || ( theta >= PI_3_8 && theta < PI_7_8) )offset = vec2(1., -1.);
    
    return offset;
}

vec3 debugAngle(vec2 offset){
    vec3 newOffset = vec3(offset, 0.);
    if(offset == vec2(-1., -1.))newOffset = vec3(0., 0., 1.);
    if(offset == vec2(1., -1.))newOffset = vec3(1., 1., 0.);
    
    return newOffset;
}


mat3 Gx = mat3( -1.0, 0.0, 1.0,
                -2.0, 0.0, 2.0,
                -1.0, 0.0, 1.0 );

mat3 Gy =  mat3( 1.0,  2.0,  1.0,
                 0.0,  0.0,  0.0,
                -1.0, -2.0, -1.0 );

float edgeDetection(sampler2DRect tex, vec2 uv, bool NonMaxAndContinuity){

    float col = 0.;
    vec2 thetaOffsetDir = vec2(0.);
    float strength = 0.;
    if(uv.x>0.&&uv.x<u_resImg.x&&uv.y>0.&&uv.y<u_resImg.y){
        
        int i,j;
        float colX = 0.;
        float colY = 0.;
        for(i = -1; i <=1; i++){
            for(j = -1; j <=1; j++){
                colX += GREYTex(tex, uv + vec2(i,j)) * (Gx[i+1][j+1]);
                colY += GREYTex(tex, uv + vec2(i,j)) * (Gy[i+1][j+1]);
            }
        }
        
        strength = pow(colX * colX + colY * colY, (.5));
        thetaOffsetDir = convertAngle(atan(colY / colX));
    }
    
    
    if(strength >= highThreshold)col = 1.0;
    if(strength < lowThreshold){
        col = 0.;
    }
    if(NonMaxAndContinuity){
        if((strength >= lowThreshold && strength < highThreshold)  ){
            bool isNeighBoorStrong = false;

            float neighboorStrength = 0.;
            int k = -1;
            for(k = -1; k <=1; k += 2){
                vec2 uvNeighboor = uv + thetaOffsetDir * k;
                
                int i,j;
                float colX = 0.;
                float colY = 0.;
                for(i = -1; i <=1; i++){
                    for(j = -1; j <=1; j++){
                        colX += GREYTex(tex, uvNeighboor + vec2(i,j)) * (Gx[i+1][j+1]);
                        colY += GREYTex(tex, uvNeighboor + vec2(i,j)) * (Gy[i+1][j+1]);
                    }
                }
                
                neighboorStrength = pow(colX * colX + colY * colY, .5);
                if(neighboorStrength > highThreshold)isNeighBoorStrong=true;
                
            }
            if( (isNeighBoorStrong || col == 1.) && neighboorStrength < strength ){

//            if( isNeighBoorStrong || (col == 1. && neighboorStrength < strength) ){
                col = 1.0;
            }else{
                col = 0.;
            }
            
        }
    }
    
    return col;
}

// ==== Canny Edge ==== //
