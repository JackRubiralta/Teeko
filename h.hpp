#include <iostream>
uint32_t numberToBITS[75710251][2];


void genNumberToBITS() {
    int64_t total = 0;
    for (int64_t m1 = 1; m1 <= 22; m1++) {
        
        
        for (int64_t m2 = m1 + 1; m2 <= 23; m2++) {
            
            for (int64_t m3 = m2 + 1; m3 <= 24; m3++) {
                
                for (int64_t m4 = m3 + 1; m4 <= 25; m4++) {
                    
                    // other color
                    for (int64_t r1 = 1; r1 <= 22; r1++) { // maybe add to 22 + m1
                        // can be 22 if not equal to m1
                        if (r1 == m1) { r1++; } // the issue is that r1 is getting added // not the issue lol
                        if (r1 == m2) { r1++; }
                        if (r1 == m3) { r1++; }
                        if (r1 == m4) { r1++; }
                        if (r1 > 22) { continue; }
                        
                        
                        for (int64_t r2 = r1 + 1; r2 <= 23; r2++) {
                            if (r2 == m1) { r2++; } 
                            if (r2 == m2) { r2++; }
                            if (r2 == m3) { r2++; }
                            if (r2 == m4) { r2++; }
                            if (r2 > 23) { continue; }

                            for (int64_t r3 = r2 + 1; r3 <= 24; r3++) {
                                if (r3 == m1) { r3++; } 
                                if (r3 == m2) { r3++; }
                                if (r3 == m3) { r3++; }
                                if (r3 == m4) { r3++; }
                                if (r3 > 24) { continue; } // this made it work


                                for (int64_t r4 = r3 + 1; r4 <= 25; r4++) {
                                    if (r4 == m1) { r4++; } 
                                    if (r4 == m2) { r4++; }
                                    if (r4 == m3) { r4++; }
                                    if (r4 == m4) { r4++; }
                                    if (r4 > 25) { continue; }
                                    

                                    total++;
                                    int64_t r11 = r1;
                                    int64_t r22 = r2;
                                    int64_t r33 = r3;
                                    int64_t r44 = r4;


                                  
                                    uint64_t t = 25;
                                    uint64_t k1 = (((t - 3) + 0) * ((t - 3) + 1) * ((t - 3) + 2) * ((t - 3) + 3)) / (4 * 3 * 2 * 1) - ((((t - 2) - m1) + 0) * (((t - 2) - m1) + 1) * (((t - 2) - m1) + 2) * (((t - 2) - m1) + 3)) / (4 * 3 * 2 * 1);
                                    uint64_t k2 = ((((t - 2) - m1) + 0) * (((t - 2) - m1) + 1) * (((t - 2) - m1) + 2)) / (3 * 2 * 1) - (((((t - 1) - m1) - (m2 - m1)) + 0) * ((((t - 1) - m1) - (m2 - m1)) + 1) * ((((t - 1) - m1) - (m2 - m1)) + 2)) / (3 * 2 * 1);
                                    uint64_t k3 = ((((t - 1) - m2) + 0) * (((t - 1) - m2) + 1)) / (2 * 1) - (((((t) - m2) - (m3 - m2)) + 0) * ((((t) - m2) - (m3 - m2)) + 1)) / (2 * 1);
                                    uint64_t k4 = (m4 - m3);

                                    if (r1 >= m1) { r11--; } if ( r1 >= m2 ) {r11--; } if ( r1 >= m3 )  {r11--; } if ( r1 >= m4 )  {r11--; } 
                                    if (r2 >= m1) { r22--; } if ( r2 >= m2 ) {r22--; } if ( r2 >= m3 )  {r22--; } if ( r2 >= m4 )  {r22--; } 
                                    if (r3 >= m1) { r33--; } if ( r3 >= m2 ) {r33--; } if ( r3 >= m3 )  {r33--; } if ( r3 >= m4 )  {r33--; } 
                                    if (r4 >= m1) { r44--; } if ( r4 >= m2 ) {r44--; } if ( r4 >= m3 )  {r44--; } if ( r4 >= m4 )  {r44--; } 
                                    // other color
                                    uint64_t s = 21;
                                    uint64_t j1 = (((s - 3) + 0) * ((s - 3) + 1) * ((s - 3) + 2) * ((s - 3) + 3)) / (4 * 3 * 2 * 1) - ((((s - 2) - r11) + 0) * (((s - 2) - r11) + 1) * (((s - 2) - r11) + 2) * (((s - 2) - r11) + 3)) / (4 * 3 * 2 * 1);
                                    uint64_t j2 = ((((s - 2) - r11) + 0) * (((s - 2) - r11) + 1) * (((s - 2) - r11) + 2)) / (3 * 2 * 1) - (((((s - 1) - r11) - (r22 - r11)) + 0) * ((((s - 1) - r11) - (r22 - r11)) + 1) * ((((s - 1) - r11) - (r22 - r11)) + 2)) / (3 * 2 * 1);
                                    uint64_t j3 = ((((s - 1) - r22) + 0) * (((s - 1) - r22) + 1)) / (2 * 1) - (((((s) - r22) - (r33 - r22)) + 0) * ((((s) - r22) - (r33 - r22)) + 1)) / (2 * 1);
                                    uint64_t j4 = (r44 - r33);


                                    
                                    uint64_t kpos = k1 + k2 + k3 + k4;
                                    uint64_t jpos = j1 + j2 + j3 + j4;
                                                                    
                                    uint64_t pos = ((kpos - 1) * 5985) + jpos;
                                    numberToBITS[total][0] = (1 << (m1 - 1)) | (1 << (m2 - 1)) | (1 << (m3 - 1)) | (1 << (m4 - 1));
                                    numberToBITS[total][1] = (1 << (r1 - 1)) | (1 << (r2 - 1)) | (1 << (r3 - 1)) | (1 << (r4 - 1));


                                    
                                    

                                    
                                
                                    
                                  

                                    

                                }
                               
                            }
                            
                            
                        } 
                             
                    }
                    //other color
                }
            }
            
        }        
    }
}


