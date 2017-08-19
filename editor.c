#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdlib.h>
#define VAR_ARR_SIZE 5
#define VAL_ARR_SIZE 11
#define GTK_STYLE_PROVIDER_PRIORITY_USER 800





GtkWidget *mainwindoweditor;
GtkWidget *editorfixed;
GtkWidget *notebook;
GtkWidget *editorlabel;


GtkWidget *step_arr_label[VAR_ARR_SIZE];
GtkWidget *label_arr[9];

GtkWidget *editor_clear_button;
GtkWidget *editor_save_button;
GtkWidget *editor_send_button;
GtkWidget *editor_start_button;
GtkWidget *editor_back_button;

GtkWidget *a1_arr[VAR_ARR_SIZE];
GtkWidget *a2_arr[VAR_ARR_SIZE];
GtkWidget *vel_arr[VAR_ARR_SIZE];
GtkWidget *acc_arr[VAR_ARR_SIZE];
GtkWidget *dec_arr[VAR_ARR_SIZE];
GtkWidget *dly_arr[VAR_ARR_SIZE];
GtkWidget *exec_arr[VAR_ARR_SIZE];
GtkWidget *rb_arr[4];
GtkWidget *rb1;



gchar *str_a1[VAR_ARR_SIZE];
gchar *str_a2[VAR_ARR_SIZE];
gchar *str_vel[VAR_ARR_SIZE];
gchar *str_acc[VAR_ARR_SIZE];
gchar *str_dec[VAR_ARR_SIZE];
gchar *str_dly[VAR_ARR_SIZE];
gchar *str_exec[VAR_ARR_SIZE];

FILE *fp;



int button_pressed;
int i;
int j;
int x;
int s;



int buff_a1[50];
int buff_a2[VAR_ARR_SIZE];
int buff_vel[VAR_ARR_SIZE];
int buff_acc[VAR_ARR_SIZE];
int buff_dec[VAR_ARR_SIZE];
int buff_dly[VAR_ARR_SIZE];
int buff_exec[VAR_ARR_SIZE];

int a1_val_alt[10] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
int a2_val_alt[10] = {0, 100, 110, 120, 130, 140, 150, 160, 170, 180};
int vel_val_alt[11] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
int acc_val_alt[11] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
int dec_val_alt[11] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
int dly_val_alt[11] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
int exec_val_alt[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int ind_val[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int ind_val2[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

char obuf_cp[17];
char ibuf_cp[17];

char obuf_sm[21];
char ibuf_sm[21];

char obuf_ed[17];
char ibuf_ed[17];


char obuf[53];
char ibuf[53];

int *buff_a1_ptr[50];
int *buff_a2_ptr[VAR_ARR_SIZE];
int *buff_vel_ptr[VAR_ARR_SIZE];
int *buff_acc_ptr[VAR_ARR_SIZE];
int *buff_dec_ptr[VAR_ARR_SIZE];
int *buff_dly_ptr[VAR_ARR_SIZE];
int *buff_exec_ptr[VAR_ARR_SIZE];

fd_set fds;
struct timeval tv;

int stepcounter;
int *stepcounterptr;

int nextmove;
int *nextmoveptr;

int locounter;
int *locounterptr;

int transid;
int *transidptr;


const gchar *a1_val[10] = {"0", "10", "20", "30", "40", "50", "60", "70", "80", "90"};
const gchar *a2_val[10]= {"0","100", "110", "120", "130", "140", "150", "160", "170", "180"};
const gchar *vel_val[11]= {"0", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100"};
const gchar *acc_val[11] = {"0","1000", "2000", "3000", "4000", "5000", "6000", "7000", "8000", "9000", "10000"};
const gchar *dec_val[11]= {"0","1000", "2000", "3000", "4000", "5000", "6000", "7000", "8000", "9000", "10000"};
const gchar *dly_val[11] = {"0", "100", "200", "300", "400", "500", "600", "700", "800", "900", "1000"};
const gchar *exec_val[11] = {"0", "1", "2", "3", "4", "5" , "6", "7", "8", "9", "10"};
const gchar *step_val[5] = {"1", "2", "3", "4", "5"};
const gchar *rb_val[4] = {"2", "3", "4", "5"};
const gchar *label_val[9] = {"#","ANGLE 1", "ANGLE 2", "VELOC. ", "ACCEL. ", "DECCEL.", "DELAY  ", "EXEC.  ", "RECIPE"};
const gchar *recipe_arr[4] = {"RECIPE 2", "RECIPE 3", "RECIPE 4", "RECIPE 5"};




static gboolean
cb_timeout( gpointer data )
{
    printf("timeout function:");
    close(s);
    exit(0);
    //return(FALSE);
}



static void check_button()
{
    button_pressed = 0;
    
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rb1)) == TRUE )
    {
        button_pressed = 1;
    }
    
    else if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rb_arr[0])) == TRUE )
    {
        button_pressed = 2;
    }
    else if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rb_arr[1])) == TRUE)
    {
        button_pressed = 3;
    }
    else if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rb_arr[2])) == TRUE)
    {
        button_pressed = 4;
    }
    else
    {
        button_pressed = 5;
    }
    printf("Chosen Recipe is number: %d\n", button_pressed);
}


void load_data(GtkWidget *widget, gpointer data)
{
    transid = 1;
    transidptr = &transid;
    
    check_button();

    printf("#0\n");
    if (button_pressed == 1)
    {
        fp = fopen("home/tkk/tkk/recipe1.txt", "r");
    }
    else if (button_pressed == 2)
    {
        fp = fopen("home/tkk/tkk/recipe2.txt", "r");
    }
    else if (button_pressed == 3)
    {
        fp = fopen("home/tkk/tkk/recipe3.txt", "r");
    }
    else if (button_pressed == 4)
    {
        fp = fopen("home/tkk/tkk/recipe4.txt", "r");
    }
    else if (button_pressed == 5)
    {
        fp = fopen("home/tkk/tkk/recipe5.txt", "r");
    }
    printf("#1\n");

    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {

        fscanf(fp, "%d",&buff_a1[i]);
        
        printf("third line in for loop\n");
        fscanf(fp, "%d",&buff_a2[i]);
        fscanf(fp, "%d",&buff_vel[i]);
        fscanf(fp, "%d",&buff_acc[i]);
        fscanf(fp, "%d",&buff_dec[i]);
        fscanf(fp, "%d",&buff_dly[i]);
        fscanf(fp, "%d",&buff_exec[i]);

        printf("el string %d: %d\n", i, buff_a1[i]);
        printf("el string %d: %d\n", i, buff_a2[i]);
        printf("el string %d: %d\n", i, buff_vel[i]);
        printf("el string %d: %d\n", i, buff_acc[i]);
        printf("el string %d: %d\n", i, buff_dec[i]);
        printf("el string %d: %d\n", i, buff_dly[i]);
        printf("el string %d: %d\n", i, buff_exec[i]);
    }
    printf("#2\n");
    
    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {

        for (j = 0; j <= 10; ++j)
        {
            if(a1_val_alt[j] == buff_a1[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(a1_arr[i]), (ind_val[j]));
            }

            if(a2_val_alt[j] == buff_a2[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(a2_arr[i]), (ind_val[j]));
            } 
        }

        for (j = 0; j <= 11; j++)
        {
            
            if(vel_val_alt[j] == buff_vel[i])
            {   
                gtk_combo_box_set_active(GTK_COMBO_BOX(vel_arr[i]), (ind_val2[j]));
            }
                            
            if(acc_val_alt[j] == buff_acc[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(acc_arr[i]), (ind_val2[j]));   
            }
               
            if(dec_val_alt[j] == buff_dec[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(dec_arr[i]), (ind_val2[j]));   
            }
                 
            if(dly_val_alt[j] == buff_dly[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(dly_arr[i]), (ind_val2[j]));     
            }
                
            if(exec_val_alt[j] == buff_exec[i])
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(exec_arr[i]), (ind_val2[j])); 
            }
                                          
        }

    }
}


void clear_table(GtkWidget *widget, gpointer data)
{
    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(a1_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(a2_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(vel_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(acc_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(dec_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(dly_arr[i]), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(exec_arr[i]), 0);
    }
    printf("Table cleared");
}


void save_data()
{
    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
        str_a1[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(a1_arr[i]));
        str_a2[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(a2_arr[i]));
        str_vel[i]= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(vel_arr[i]));
        str_acc[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(acc_arr[i]));
        str_dec[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(dec_arr[i]));
        str_dly[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(dly_arr[i]));
        str_exec[i] = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(exec_arr[i]));
    }
    check_button();

    if (button_pressed == 1)
    {
        fp = fopen("home/tkk/tkk/recipe1.txt", "w+");
        printf("opened recipe: 1");
    }
    else if (button_pressed == 2)
    {
        fp = fopen("home/tkk/tkk/recipe2.txt", "w+");
        printf("opened recipe: 2");
    }
    else if (button_pressed == 3)
    {
        fp = fopen("home/tkk/tkk/recipe3.txt", "w+");
        printf("opened recipe: 3");
    }
    else if (button_pressed == 4)
    {
        fp = fopen("home/tkk/tkk/recipe4.txt", "w+");
        printf("opened recipe: 4");
    }
    else
    {
        fp = fopen("home/tkk/tkk/recipe5.txt", "w+");
        printf("opened recipe: 5");
    }


    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
        fputs(str_a1[i], fp);
        fputs("\n", fp); 
        fputs(str_a2[i], fp);
        fputs("\n", fp); 
        fputs(str_vel[i], fp);
        fputs("\n", fp); 
        fputs(str_acc[i], fp);
        fputs("\n", fp); 
        fputs(str_dec[i], fp);
        fputs("\n", fp); 
        fputs(str_dly[i], fp);
        fputs("\n", fp); 
        fputs(str_exec[i], fp);
        fputs("\n", fp); 
    }

    fclose(fp);

}

void send_data(GtkWidget *widget, gpointer user_data)
{
    check_button();
    char *button_pressed_buff;
    sprintf(button_pressed_buff, "%d", button_pressed);


    

    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
        fscanf(fp, "%d",&buff_a1[i]);
        fscanf(fp, "%d",&buff_a2[i]);
        fscanf(fp, "%d",&buff_vel[i]);
        fscanf(fp, "%d",&buff_acc[i]);
        fscanf(fp, "%d",&buff_dec[i]);
        fscanf(fp, "%d",&buff_dly[i]);
        fscanf(fp, "%d",&buff_exec[i]);

        printf("el string %d: %d\n", i, buff_a1[i]);
        printf("el string %d: %d\n", i, buff_a2[i]);
        printf("el string %d: %d\n", i, buff_vel[i]);
        printf("el string %d: %d\n", i, buff_acc[i]);
        printf("el string %d: %d\n", i, buff_dec[i]);
        printf("el string %d: %d\n", i, buff_dly[i]);
        printf("el string %d: %d\n", i, buff_exec[i]);
    }

    fclose(fp);

    ////////////////////////////////
    ///// CLEARING ALL POSITIONS ////
    locounter = 0;
    locounterptr = &locounter;
    for (i = 0; i < 128; ++i)
    {
        
        int * intloc1_cp = (int*)(&obuf_cp[0]);
        int * intloc2_cp = (int*)(&obuf_cp[2]);
        int * intloc3_cp = (int*)(&obuf_cp[4]);
        int * intloc4_cp = (int*)(&obuf_cp[6]);
        int * intloc5_cp = (int*)(&obuf_cp[7]);
        int * intloc6_cp = (int*)(&obuf_cp[8]);
        int * intloc7_cp = (int*)(&obuf_cp[10]);
        int * intloc8_cp = (int*)(&obuf_cp[12]);
        int * intloc9_cp = (int*)(&obuf_cp[13]);
            
        * intloc1_cp = htons(*transidptr);   ///// (*spremenljivka)    | * intloc1 = htons(arr_);
        * intloc2_cp = htons(0);
        * intloc3_cp = htons(11);
        * intloc4_cp = 1;
        * intloc5_cp = 16;
        * intloc6_cp = htons(530);
        * intloc7_cp = htons(2);
        * intloc8_cp = 4;
        * intloc9_cp = htonl(*locounterptr);
            
        FD_ZERO(&fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
            
        x = select(32, NULL, &fds, NULL, &tv);
        x = send(s, obuf_cp, 17, 0);
        printf("Message Sent!\n");
        FD_SET(s, &fds);
        x = select(32, &fds, NULL, NULL, &tv);
        x = recv(s, ibuf_cp, 54 , 0);
        printf("Message Received \n");
            
        printf("Position: %d cleared\n", locounter );
        
        locounter++;
        transid++;

        printf("///////////LOCOUNTER PTR : %d\n", *locounterptr);
    }

    for (i = 0;i < VAR_ARR_SIZE; ++i)
    {
        buff_a1_ptr[i] = &buff_a1[i];
        buff_a2_ptr[i] = &buff_a2[i];
        buff_vel_ptr[i] = &buff_vel[i];
        buff_acc_ptr[i] = &buff_acc[i];
        buff_dec_ptr[i] = &buff_dec[i];
        buff_dly_ptr[i] = &buff_dly[i];   
        buff_exec_ptr[i] = &buff_exec[i];

        buff_a1[i] = buff_a1[i] * 51000;
        buff_a2[i] = buff_a2[i] * 51000;
        buff_vel[i] = buff_vel[i] * 30000;
        buff_acc[i] = buff_acc[i] * 1000;
        buff_dec[i] = buff_dec[i] * 1000;
    }

    stepcounterptr = 0;
    stepcounterptr = &stepcounter;

    nextmove = 1;
    nextmoveptr = &nextmove;

    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
        if(buff_exec[i] > 0)
        {
            for (j = 0; j < buff_exec[i]; j++)
            {
                int * intloc1 = (int*)(&obuf[0]);
                int * intloc2 = (int*)(&obuf[2]);
                int * intloc3 = (int*)(&obuf[4]);
                int * intloc4 = (int*)(&obuf[6]);
                int * intloc5 = (int*)(&obuf[7]);
                int * intloc6 = (int*)(&obuf[8]);
                int * intloc7 = (int*)(&obuf[10]);
                int * intloc8 = (int*)(&obuf[12]);
                int * intloc9 = (int*)(&obuf[13]);
                int * intloc10 = (int*)(&obuf[17]);
                int * intloc11 = (int*)(&obuf[21]);
                int * intloc12 = (int*)(&obuf[25]);
                int * intloc13 = (int*)(&obuf[29]);
                int * intloc14 = (int*)(&obuf[33]);
                int * intloc15 = (int*)(&obuf[37]);
                int * intloc16 = (int*)(&obuf[41]);
                int * intloc17 = (int*)(&obuf[45]);
                int * intloc18 = (int*)(&obuf[49]);

                * intloc1 = htons(*transidptr);   ///// (*spremenljivka)    | * intloc1 = htons(arr_);
                * intloc2 = htons(0);
                * intloc3 = htons(47);
                * intloc4 = 1;
                * intloc5 = 16;
                * intloc6 = htons(8192);
                * intloc7 = htons(20);
                * intloc8 = 40;
                * intloc9 = htonl(*stepcounterptr);       ///// (*spremenljivka)    | * intloc1 = htons(arr_);
                * intloc10 = htonl(*buff_a1_ptr[i]);
                * intloc11 = htonl(*buff_vel_ptr[i]);
                
                if (buff_dly[i] == 0 )
                {
                    * intloc12 = htonl(16);
                }
                else 
                {
                    * intloc12 = htonl(48);   
                }

                * intloc13 = htonl(*buff_acc_ptr[i]);  ///// (*spremenljivka)    | * intloc13 = htons(acc_arr[i]);
                * intloc14 = htonl(*buff_dec_ptr[i]);    ////// CNTL NUMBER /////
                * intloc15 = htonl(*nextmoveptr);
                * intloc16 = htonl(*buff_dly_ptr[i]);
                * intloc17 = htonl(1);
                * intloc18 = htonl(1);


                FD_ZERO(&fds);
                tv.tv_sec = 0;
                tv.tv_usec = 0;

                              
                x = select(32, NULL, &fds, NULL, &tv);
                x = send(s, obuf, 53, 0);
                printf("Message Sent!\n");
                FD_SET(s, &fds);
                x = select(32, &fds, NULL, NULL, &tv);
                x = recv(s, ibuf, 54 , 0);
                printf("Message Received \n");

                
                printf("Position: %d saved\n", stepcounter );
                stepcounter++;
                transid++;
                nextmove++;


                * intloc1 = htons(*transidptr);   ///// (*spremenljivka)    | * intloc1 = htons(arr_);
                * intloc2 = htons(0);
                * intloc3 = htons(47);
                * intloc4 = 1;
                * intloc5 = 16;
                * intloc6 = htons(8192);
                * intloc7 = htons(20);
                * intloc8 = 40;
                * intloc9 = htonl(*stepcounterptr);       ///// (*spremenljivka)    | * intloc1 = htons(arr_);
                * intloc10 = htonl(*buff_a2_ptr[i]);
                * intloc11 = htonl(*buff_vel_ptr[i]);
                
                if (buff_dly[i] == 0 )
                {
                    * intloc12 = htonl(16);
                }
                else 
                {
                    * intloc12 = htonl(48);   
                }

                * intloc13 = htonl(*buff_acc_ptr[i]);  ///// (*spremenljivka)    | * intloc13 = htons(acc_arr[i]);
                * intloc14 = htonl(*buff_dec_ptr[i]);    ////// CNTL NUMBER /////
                * intloc15 = htonl(*nextmoveptr);
                * intloc16 = htonl(*buff_dly_ptr[i]);
                * intloc17 = htonl(1);
                * intloc18 = htonl(1);


                FD_ZERO(&fds);
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                               
                x = select(32, NULL, &fds, NULL, &tv);
                x = send(s, obuf, 53, 0);
                printf("Message Sent!\n");
                FD_SET(s, &fds);
                x = select(32, &fds, NULL, NULL, &tv);
                x = recv(s, ibuf, 54 , 0);
                printf("Message Received \n");
                printf("Position: %d saved\n", stepcounter );
                stepcounter++;
                transid++;
                nextmove++;
            }
        }

    }
    /////////////// HOME POSITION /////////////////////
    int * intloc1 = (int*)(&obuf[0]);
    int * intloc2 = (int*)(&obuf[2]);
    int * intloc3 = (int*)(&obuf[4]);
    int * intloc4 = (int*)(&obuf[6]);
    int * intloc5 = (int*)(&obuf[7]);
    int * intloc6 = (int*)(&obuf[8]);
    int * intloc7 = (int*)(&obuf[10]);
    int * intloc8 = (int*)(&obuf[12]);
    int * intloc9 = (int*)(&obuf[13]);
    int * intloc10 = (int*)(&obuf[17]);
    int * intloc11 = (int*)(&obuf[21]);
    int * intloc12 = (int*)(&obuf[25]);
    int * intloc13 = (int*)(&obuf[29]);
    int * intloc14 = (int*)(&obuf[33]);
    int * intloc15 = (int*)(&obuf[37]);
    int * intloc16 = (int*)(&obuf[41]);
    int * intloc17 = (int*)(&obuf[45]);
    int * intloc18 = (int*)(&obuf[49]);

    * intloc1 = htons(*transidptr);   ///// (*spremenljivka)    | * intloc1 = htons(arr_);
    * intloc2 = htons(0);
    * intloc3 = htons(47);
    * intloc4 = 1;
    * intloc5 = 16;
    * intloc6 = htons(8192);
    * intloc7 = htons(20);
    * intloc8 = 40;
    * intloc9 = htonl(*stepcounterptr);       ///// (*spremenljivka)    | * intloc1 = htons(arr_);
    * intloc10 = htonl(50000);
    * intloc11 = htonl(300000);
    * intloc12 = htonl(16);
    * intloc13 = htonl(100000);  ///// (*spremenljivka)    | * intloc13 = htons(acc_arr[i]);
    * intloc14 = htonl(100000);    ////// CNTL NUMBER /////
    * intloc15 = htonl(*nextmoveptr);
    * intloc16 = htonl(0);
    * intloc17 = htonl(1);
    * intloc18 = htonl(1);


    FD_ZERO(&fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    x = select(32, NULL, &fds, NULL, &tv);
    x = send(s, obuf, 53, 0);
    printf("Message Sent!\n");
    FD_SET(s, &fds);
    x = select(32, &fds, NULL, NULL, &tv);
    x = recv(s, ibuf, 54 , 0);
    printf("Message Received \n");
    
    printf("Position: %d saved\n", stepcounter );
    
    transid++;
    stepcounter = 0;
}

void start_motion()
{
    int * intloc1_sm = (int*)(&obuf_sm[0]);
    int * intloc2_sm = (int*)(&obuf_sm[2]);
    int * intloc3_sm = (int*)(&obuf_sm[4]);
    int * intloc4_sm = (int*)(&obuf_sm[6]);
    int * intloc5_sm = (int*)(&obuf_sm[7]);
    int * intloc6_sm = (int*)(&obuf_sm[8]);
    int * intloc7_sm = (int*)(&obuf_sm[10]);
    int * intloc8_sm = (int*)(&obuf_sm[12]);
    int * intloc9_sm = (int*)(&obuf_sm[13]);
    int * intloc10_sm = (int*)(&obuf_sm[17]);


    * intloc1_sm = htons(*transidptr);   ///// (*spremenljivka)    | * intloc1 = htons(arr_);
    * intloc2_sm = htons(0);
    * intloc3_sm = htons(15);
    * intloc4_sm = 1;
    * intloc5_sm = 16;
    * intloc6_sm = htons(8212);
    * intloc7_sm = htons(4);
    * intloc8_sm = 8;
    * intloc9_sm = htonl(1);
    * intloc10_sm = htonl(0);


    FD_ZERO(&fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    x = select(32, NULL, &fds, NULL, &tv);
    x = send(s, obuf_sm, 21, 0);
    printf("Message Sent!\n");
    FD_SET(s, &fds);
    x = select(32, &fds, NULL, NULL, &tv);
    x = recv(s, ibuf_sm, 54 , 0);
    printf("STARTING MOTION \n");
    transid++;
}

void enable_drive()
{
    ////////////////////// ENABLE DRIVE /////////////////
    int * intloc1_ed = (int*)(&obuf_ed[0]);
    int * intloc2_ed = (int*)(&obuf_ed[2]);
    int * intloc3_ed = (int*)(&obuf_ed[4]);
    int * intloc4_ed = (int*)(&obuf_ed[6]);
    int * intloc5_ed = (int*)(&obuf_ed[7]);
    int * intloc6_ed = (int*)(&obuf_ed[8]);
    int * intloc7_ed = (int*)(&obuf_ed[10]);
    int * intloc8_ed = (int*)(&obuf_ed[12]);
    int * intloc9_ed = (int*)(&obuf_ed[13]);
            
    * intloc1_ed = htons(0);  
    * intloc2_ed = htons(0);
    * intloc3_ed = htons(11);
    * intloc4_ed = 1;
    * intloc5_ed = 16;
    * intloc6_ed = htons(254);
    * intloc7_ed = htons(2);
    * intloc8_ed = 4;
    * intloc9_ed = htonl(1);
            
    FD_ZERO(&fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
            
    x = select(32, NULL, &fds, NULL, &tv);
    x = send(s, obuf_ed, 17, 0);
            
    FD_SET(s, &fds);
    x = select(32, &fds, NULL, NULL, &tv);
    x = recv(s, ibuf_ed, 50 , 0);
    transid++;        
            
    printf("Drive Enabled\n" );
}


int main(int argc, char* argv[])
{


	gtk_init(&argc, &argv);



    //////////////////////////////////
    ////////////////////////////////
    mainwindoweditor = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    notebook = 	gtk_notebook_new();
    editorfixed = gtk_fixed_new();
    editorlabel = gtk_label_new("EDITOR");
    ///////////////////////////////////////////////////

    gtk_widget_set_size_request(mainwindoweditor, 1024, 600);
    gtk_window_set_resizable (GTK_WINDOW(mainwindoweditor), FALSE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), editorfixed, editorlabel);
    gtk_container_add(GTK_CONTAINER(mainwindoweditor), notebook);
    ////////////////////////////////////////////////////////
    //////// CSS /////////////////////
    //GtkCssProvider *cssProvider = gtk_css_provider_new();
    //gtk_css_provider_load_from_path(cssProvider, "home/pi/tkk/style.css", NULL);
    //gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    ////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    int a1_x, all_y, a2_x, vel_x, acc_x, dec_x, dly_x, exec_x, step_x, step_y, width, height;
    width = 100;
    height = 50;
    a1_x =  55;
    a2_x = 175;
    vel_x = 295;
    acc_x = 415;
    dec_x = 535;
    dly_x = 655;
    exec_x = 775;
    step_x = 20;
    step_y = 100;

    all_y = 90;
    
    for (i = 0; i < VAR_ARR_SIZE; ++i)
    {
    	a1_arr[i] = gtk_combo_box_text_new ();
    	a2_arr[i] = gtk_combo_box_text_new ();
    	vel_arr[i] = gtk_combo_box_text_new ();
    	acc_arr[i] = gtk_combo_box_text_new ();
    	dec_arr[i] = gtk_combo_box_text_new ();
    	dly_arr[i] = gtk_combo_box_text_new ();
    	exec_arr[i] = gtk_combo_box_text_new ();


        for (j = 0; j < 11; ++j )
        {

            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(vel_arr[i]), NULL, vel_val[j]);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(dly_arr[i]), NULL, dly_val[j]);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(exec_arr[i]), NULL, exec_val[j]);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(acc_arr[i]), NULL, acc_val[j]);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(dec_arr[i]), NULL, dec_val[j]);

        }

        for (j = 0; j < 10; ++j )
        {
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(a1_arr[i]), NULL, a1_val[j]);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(a2_arr[i]), NULL, a2_val[j]);
        }
        
        step_arr_label[i] = gtk_label_new(step_val[i]);   
	
    	gtk_fixed_put (GTK_FIXED (editorfixed), a1_arr[i], a1_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), a2_arr[i], a2_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), vel_arr[i], vel_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), acc_arr[i], acc_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), dec_arr[i], dec_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), dly_arr[i], dly_x, all_y);
    	gtk_fixed_put (GTK_FIXED (editorfixed), exec_arr[i], exec_x, all_y);
        gtk_fixed_put (GTK_FIXED (editorfixed), step_arr_label[i], step_x, step_y);

        gtk_widget_set_size_request(a1_arr[i], width, height);
        gtk_widget_set_size_request(a2_arr[i], width, height);
        gtk_widget_set_size_request(vel_arr[i], width, height);
        gtk_widget_set_size_request(acc_arr[i], width, height);
        gtk_widget_set_size_request(dec_arr[i], width, height);
        gtk_widget_set_size_request(dly_arr[i], width, height);
        gtk_widget_set_size_request(exec_arr[i], width, height);
    	
        all_y =	all_y + 60;
        step_y = step_y + 60;
    }

    int label_x, label_y;
    label_x = 60;
    label_y = 30;
    
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////// RADIO BUTTONS ////////////////////////////////////
    int rb_x, rb_y;
    rb_x = 920;
    rb_y = 150;
    rb1 = gtk_radio_button_new_with_label(NULL, "1");
    gtk_fixed_put (GTK_FIXED (editorfixed), rb1, 920, 90);
    g_signal_connect (rb1, "clicked", G_CALLBACK (load_data), NULL );
    gtk_widget_set_size_request(rb1, width, height);

    for(i = 0; i < 4; ++i )
    {
        rb_arr[i] = gtk_radio_button_new_with_label(gtk_radio_button_get_group (GTK_RADIO_BUTTON(rb1)), rb_val[i]);
        gtk_fixed_put (GTK_FIXED (editorfixed), rb_arr[i], rb_x, rb_y);
        gtk_widget_set_size_request(rb_arr[i], width, height);
        g_signal_connect (rb_arr[i], "clicked", G_CALLBACK (load_data), NULL );
        rb_y = rb_y + 60;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    /////////// LABELS ///////////////////////////////////////////////
    
    for (i = 0; i < 1; ++i)
    {
        label_arr[i] = gtk_label_new(label_val[i]);
        gtk_fixed_put (GTK_FIXED (editorfixed), label_arr[i], 20, 30);
    }


    for (i = 1; i < 9; ++i)
    {
        label_arr[i] = gtk_label_new(label_val[i]);
        gtk_fixed_put (GTK_FIXED (editorfixed), label_arr[i], label_x, label_y);
        label_x = label_x + 120;

    }  

    ////////////////////////////////////////////////////////////////////    
    ////////////// BUTTONS /////////////////////////////////////////////
    editor_clear_button = gtk_button_new_with_label ("CLEAR");
    editor_save_button = gtk_button_new_with_label ("SAVE");
    editor_send_button = gtk_button_new_with_label ("SEND");
    editor_start_button = gtk_button_new_with_label ("START");
    editor_back_button = gtk_button_new_with_label ("BACK");
   
    gtk_widget_set_size_request(editor_back_button, 200, 100);
     

    g_signal_connect (editor_clear_button, "clicked", G_CALLBACK (clear_table), NULL);
    g_signal_connect (editor_save_button, "clicked", G_CALLBACK (save_data), NULL );
    g_signal_connect (editor_send_button, "clicked", G_CALLBACK (send_data), NULL);
    g_signal_connect (editor_start_button, "clicked", G_CALLBACK (start_motion), NULL);
    g_signal_connect( G_OBJECT( editor_back_button ), "clicked", G_CALLBACK( cb_timeout ), NULL );  

    gtk_fixed_put (GTK_FIXED (editorfixed), editor_clear_button,     150, 450);
    gtk_fixed_put (GTK_FIXED (editorfixed), editor_save_button,     900, 450);
    gtk_fixed_put (GTK_FIXED (editorfixed), editor_send_button,     415, 450);
    gtk_fixed_put (GTK_FIXED (editorfixed), editor_start_button,     535, 450);
    gtk_fixed_put (GTK_FIXED (editorfixed), editor_back_button,     55, 450);

    gtk_widget_set_size_request(editor_clear_button, 100, 70);
    gtk_widget_set_size_request(editor_save_button, 50, 70);
    gtk_widget_set_size_request(editor_send_button, 100, 70);
    gtk_widget_set_size_request(editor_start_button, 235, 70);
    gtk_widget_set_size_request(editor_back_button, 75, 70);

    /////////////////////////////////////////////////////////
    ///////////////// CONNECTION SETTINGS ///////////////////
    char *ip_adrs;
    struct sockaddr_in server;
    ip_adrs = "192.168.0.35";
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.sin_family = AF_INET;
    server.sin_port = htons(502);
    server.sin_addr.s_addr = inet_addr(ip_adrs);

    //x = connect(s, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    printf("x = %d\n", x);
    if (x<0)
    {
        printf("connect - error %d\n", x);
        close(s);
    return 1;
    }
    else
    {
        printf("Connection successful\n");
    }
    /////////////////////////////////////////////////////7
    
   // load_data(editor_save_button, "clicked");
    printf("after\n");
    /////////////////////////////////////////////////////


    //////////////////////////////////////////////////////
    //g_idle_add( cb_idle, (gpointer)status_button );
    //gdk_threads_add_timeout_full( 800,3000, cb_timeout, (gpointer)status_button,NULL );
    ///////////////////  MAIN GTK LOOP /////////////////
    gtk_widget_show_all(mainwindoweditor);
    gtk_main();
    
    return 0;

}







