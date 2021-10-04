#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "PJ_RPI.h"
#include <stdio.h>

int main()
{
    if(map_peripheral(&gpio) == -1) 
    {
            printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
            return -1;
        }
    int pressed = 0;
    while(1)
    {
        int active = GPIO_READ(27);
        if(active > 0 && !pressed){
            pressed = 1;
            MYSQL *con = mysql_init(NULL);
            if (con == NULL) 
            {
                fprintf(stderr, "Connectie error:%s\n", mysql_error(con));
                exit(1);
            }
            if (mysql_real_connect(con, "localhost", "pi", "raspberry", "io_log", 0, NULL, 0) == NULL) 
            {
                fprintf(stderr, "Error bij connectie: %s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
            }
            printf("boop\n");
            if (mysql_query(con, "INSERT INTO logs (knop) VALUES (1)"))
            {
                finish_with_error(con);
            }
            sleep(1);
            mysql_close(con);
        }else if(active == 0 && pressed)
        {
            printf("%d %d\n",active,pressed);
            pressed = 0;
        }
        
    }  
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "Finish with error:%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
