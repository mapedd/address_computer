#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <stdbool.h>

enum read_out_dir {
    read_down_left = 1,
    read_down_right = 2,
    read_up_left = 3,
    read_up_right = 4
};

int main(int argc, char *argv[]) {


    if(argc < 6){
        fprintf(stderr, "Not enough arguments! Usage: h v h_s v_s read_out_type [-dl,-dr,-ul,-ur] print all addresses to stdout/file[-p/-f] number_of_frames\n");
        return 0;
    }
    
    
    int read_out_type;
    
    if(strcmp(argv[5],"-dl") == 0){
        read_out_type = read_down_left;
    }
    else if(strcmp(argv[5],"-dr") == 0){
        read_out_type = read_down_right;
    }
    else if(strcmp(argv[5],"-ul") == 0){
        read_out_type = read_up_left;
    }
    else if(strcmp(argv[5],"-ur") == 0){
        read_out_type = read_up_right;
    }
    else{
        read_out_type = read_down_right;
        printf("You didn't provide read direction, assuming : down right.");
    }
    
    
    bool canPrintToStdOut = false;
    bool canPrintToFile = false;
    
    FILE *stream;
    
    if (argc >= 7 ){
        if (strcmp(argv[6],"-f") == 0){
            canPrintToFile= true;
            stream = fopen("log.txt", "w");
        }
            else if(strcmp(argv[6],"-p") == 0){
            canPrintToStdOut = true;
            stream = stdout;
        }
    }

    
    int h = atoi(argv[1]);
    int v = atoi(argv[2]);
    int h_s = atoi(argv[3]);
    int v_s = atoi(argv[4]);

    int total_pixels = h * v;

    int address_offset = 0;
    
    // frame counter if user want more than one frame to be printed
    int f_counter = 1;
    
    if (argc >= 8){
       f_counter = atoi(argv[7]);
    }
    
    printf("Arguments:                   \n");
    printf("  resolution    horizontal:%d\n", h);
    printf("                  vertical:%d\n",v);
    printf("  segments:     horizontal:%d\n",h_s);
    printf("                  vertical:%d\n\n",v_s);
    
    if(canPrintToFile){
        fprintf(stream, "Arguments:                   \n");
        fprintf(stream, "  resolution    horizontal:%d\n", h);
        fprintf(stream, "                  vertical:%d\n",v);
        fprintf(stream, "  segments:     horizontal:%d\n",h_s);
        fprintf(stream, "                  vertical:%d\n\n",v_s);
    }
    
    int h_x = h/h_s; // segment width
    int v_y = v/v_s; // segment height
    int s_l = (h * v) / (h_s * v_s);    // segment length
    int counter = 0;
    
    if(h_x * h_s != h){ 
        printf("Horizontal resolution must be dividible by the number of horizonral segments!\n");
        return 0;
    }
    
    if(v_y * v_s != v){
        printf("Vertical resolution must be dividible by the number of vertical segments!\n");
        return 0;
    }
    
    int i,j,k;
    
    for (k = 0; k < f_counter; ++k){

        for (i=0;i<v_s;i++) {

            int n;
            
            for (j=0;j<h_s;j++) {
                
                
                int m;
                
                switch (read_out_type) {
                    case read_down_right:
                    
                        n = v_y * i;
                        m = h_x * j;
                        
                    break;
                    case read_down_left:
                                    
                        n = v_y * i;
                        m = h_x * j + (h_x - 1);
                        
                    break;
                    case read_up_right:
                    
                        n = v_y * i + (v_y - 1);
                        m = h_x * j;
                        
                    break;
                    case read_up_left:
                    
                        n = v_y * i + (v_y - 1);
                        m = h_x * j + (h_x - 1);
                        
                    break;
                    default:
                        m = 0;
                        n = 0;
                    break;
                }

                
                int index = n * h + m;      
                
                
                
                printf("\ni = %d, A = (%d,%d) index = %d\n",counter,n,m, index);
                counter++;
                
                if(canPrintToFile){
                   fprintf(stream, "\ni = %d, A = (%d,%d) index = %d\n",counter,n,m, index);   
                }
                
                if(canPrintToStdOut || canPrintToFile){
                    int k = 0;
                                
                    switch (read_out_type) {
                        case read_down_right:{
                            int width_counter = 0;
                            
                            while(k<s_l){
                                
                                if (width_counter<h_x) {
                                    int x = index+width_counter + address_offset;
                                    fprintf(stream, " %d \t",x);
                                    if(x<10) fprintf(stream, "\t");
                                    width_counter ++;
                                    k++;
                                }
                                else{
                                    fprintf(stream, "\n");
                                    width_counter = 0;
                                    n++;
                                    index = n * h + m;
                                }
                            }
                        }
                        break;
                        case read_down_left:{
                            int width_counter = 0;
                            
                            while(k<s_l){
                                
                                if (width_counter<h_x) {
                                    int x = index-width_counter + address_offset;
                                    fprintf(stream, " %d \t",x);
                                    if(x<10) fprintf(stream, "\t");
                                    width_counter ++;
                                    k++;
                                }
                                else{
                                    fprintf(stream, "\n");
                                    width_counter = 0;
                                    n++;
                                    index = n * h + m;
                                }
                            }
                        }               
                        break;
                        case read_up_right:{
                            int width_counter = 0;
                            
                            while(k<s_l){
                                
                                if (width_counter<h_x) {
                                    int x = index+width_counter + address_offset;
                                    fprintf(stream, " %d \t",x);
                                    if(x<10) fprintf(stream, "\t");
                                    width_counter ++;
                                    k++;
                                }
                                else{
                                    fprintf(stream, "\n");
                                    width_counter = 0;
                                    n--;
                                    index = n * h + m;
                                }
                            }
                        }
                            
                        break;
                        case read_up_left:{
                            int width_counter = 0;
                            
                            while(k<s_l){
                                
                                if (width_counter<h_x) {
                                    int x = index-width_counter + address_offset;
                                    fprintf(stream, " %d \t",x);
                                    if(x<10) fprintf(stream, " ");
                                    width_counter ++;
                                    k++;
                                }
                                else{
                                    fprintf(stream, "\n");
                                    width_counter = 0;
                                    n--;
                                    index = n * h + m;
                                }
                            }
                        }
                        break;
                        default:
                        
                        break;
                     }
                 }
                
                if(m >= v) break;
            }
            
            if (n >= h) break;
        }

        address_offset += total_pixels;


        if(canPrintToFile || canPrintToStdOut){
            if (f_counter > 1)
            {
                fprintf(stream, "\n----------------------------------------------\n");
            }
        }

    }
    
    fprintf(stream, "\n");
    
    system("pause");
    
    return 0;
}
