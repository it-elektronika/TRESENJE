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



GtkWidget *mainwindow;
GtkWidget *statusfixed;
GtkWidget *notebook;
GtkWidget *statuslabel;
GtkWidget *status_button;
GtkWidget *editor_button;


FILE *fp;

int button_pressed;
int i;
int j;
int x;
int s;






static gboolean
cb_timeout( gpointer data )
{
    
    //close(s);
    //int status = system("./editor");
    sleep(1);
    return(TRUE);
    
    
}



int main(int argc, char* argv[])
{


	gtk_init(&argc, &argv);



    //////////////////////////////////
    ////////////////////////////////
    mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    notebook = 	gtk_notebook_new();
    statusfixed = gtk_fixed_new();
    statuslabel = gtk_label_new("STATUS");
    ///////////////////////////////////////////////////

    gtk_widget_set_size_request(mainwindow, 1024, 600);
    gtk_window_set_resizable (GTK_WINDOW(mainwindow), FALSE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), statusfixed, statuslabel);
    gtk_container_add(GTK_CONTAINER(mainwindow), notebook);
    ////////////////////////////////////////////////////////
    //////// CSS /////////////////////
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "home/tkk/tkk/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    


    status_button = gtk_button_new_with_label ("ON");
    gtk_fixed_put (GTK_FIXED (statusfixed), status_button,     400, 100);
    gtk_widget_set_size_request(status_button, 200, 100);


    editor_button = gtk_button_new_with_label ("EDITOR");
    gtk_fixed_put (GTK_FIXED (statusfixed), editor_button,     400, 300);
    gtk_widget_set_size_request(editor_button, 200, 100);
    g_signal_connect( G_OBJECT( editor_button ), "clicked",
                      G_CALLBACK( cb_timeout ), NULL );

    ////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    ///////////////// CONNECTION SETTINGS ///////////////////

    // char *ip_adrs;
    // struct sockaddr_in server;
    // ip_adrs = "127.0.0.1";
    // s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // server.sin_family = AF_INET;
    // server.sin_port = htons(502);
    // server.sin_addr.s_addr = inet_addr(ip_adrs);

    // x = connect(s, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

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
    /////////////////////////////////////////////////////
    //g_idle_add( cb_idle, (gpointer)status_button );
    //gdk_threads_add_timeout_full( 800,3000, cb_timeout, (gpointer)status_button,NULL );
    ///////////////////  MAIN GTK LOOP /////////////////
    gtk_widget_show_all(mainwindow);
    gtk_main();
    
    return 0;

}







