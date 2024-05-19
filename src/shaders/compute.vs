#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

//#extension GL_ARB_compute_shader : enable;
//#extension GL_ARB_shader_storage_buffer_object : enable;

uniform float delta = 1.0f; 

layout (std430, binding = 4) buffer pos {
    vec2 positions[]; 
};

layout(std430, binding = 5) buffer meta {
    vec4 metaballs[];
}; 

layout(std430, binding = 6) buffer outputpositions {
    vec2 outputPositions[];
};

float computeAtXY(float x, float y){
    float sum = 0; 
    for(int i = 0; i < metaballs.length(); i++){
        sum += metaballs[i].z/distance(metaballs[i].xy, vec2(x, y)); 
    }
    return sum; 
}

void main(){
    uint gid = gl_GlobalInvocationID.x;
    float centerValue; 
    bool tl, tr, bl, br, c; 

    outputPositions[gid*4] =  vec2(-1, -1); 
    outputPositions[gid*4 + 1] = vec2(-2, -1);
    outputPositions[gid*4 + 2] = vec2(-3, -1);
    outputPositions[gid*4 + 3] = vec2(-4, -1);

    // Center calculation
    if (computeAtXY(positions[gid].x, positions[gid].y) >= 1){
        c = true;  
    } else {
        c = false; 
    }
    
    // Top right 
    if (computeAtXY(positions[gid].x + delta/2, positions[gid].y + delta/2) >= 1){
        tr = true;  
    } else {
        tr = false; 
    }

    // Top left 
    if (computeAtXY(positions[gid].x - delta/2, positions[gid].y + delta/2) >= 1){
        tl = true;  
    } else {
        tl = false; 
    }

    // Bottom right
    if (computeAtXY(positions[gid].x + delta/2, positions[gid].y - delta/2) >= 1){
        br = true;  
    } else {
        br = false; 
    }

    // Bottom left 
    if (computeAtXY(positions[gid].x - delta/2, positions[gid].y - delta/2) >= 1){
        bl = true;  
    } else {
        bl = false; 
    }

    if (br == true && bl == false && tl == false && tr == false){
        // Marching cubes
        outputPositions[gid*4] =        vec2(positions[gid].x + delta/2, positions[gid].y);
        outputPositions[gid*4 + 1] =    vec2(positions[gid].x, positions[gid].y - delta/2);

    }
    if (br == false && bl == true && tl == false && tr == false){
        // Marching cubes
        outputPositions[gid*4] =        vec2(positions[gid].x - delta/2, positions[gid].y);
        outputPositions[gid*4 + 1] =    vec2(positions[gid].x, positions[gid].y - delta/2);
    }
    if (br == false && bl == false && tl == true && tr == false){
        // Marching cubes
        outputPositions[gid*4] =        vec2(positions[gid].x - delta/2, positions[gid].y);
        outputPositions[gid*4 + 1] =    vec2(positions[gid].x, positions[gid].y + delta/2);

    }
    if (br == false && bl == false && tl == false && tr == true){
        // Marching cubes
        outputPositions[gid*4] =        vec2(positions[gid].x + delta/2, positions[gid].y);
        outputPositions[gid*4 + 1] =    vec2(positions[gid].x, positions[gid].y + delta/2);
    }
    
    // if (br == true && bl == true && tl == true && tr == true){
    //     // Marching cubes
    //     outputPositions[gid*4] = vec2(gid, metaballs.length());
    //     outputPositions[gid*4 + 1] =    vec2(computeAtXY(positions[gid].x - delta/2, positions[gid].y - delta/2), 808);
    //     outputPositions[gid*4 + 2] =    vec2(positions[gid].x, positions[gid].y);
    // }
    
    if (br == false && bl == false && tl == false && tr == false){
        // Marching cubes
        outputPositions[gid*4] = vec2(gid, metaballs.length());
        outputPositions[gid*4 + 1] =    vec2(computeAtXY(positions[gid].x - delta/2, positions[gid].y - delta/2), 808);
        outputPositions[gid*4 + 2] =    vec2(positions[gid].x, positions[gid].y);
    }

    positions[gid] = vec2(metaballs[0].x, metaballs[0].y);
}