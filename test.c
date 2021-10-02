#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
int main(int argc, char **argv)
{  
  MYSQL *con = mysql_init(NULL);
  printf("MySQL client version: %s\n", mysql_get_client_info());
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }
  if (mysql_real_connect(con, "localhost", "pi", "raspberry", 
            "globe_bank", 0, NULL, 0) == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }  
 if (mysql_query(con, "SELECT * FROM subjects"))
  {
      finish_with_error(con);
  }
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL)
  {
      finish_with_error(con);
  }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields; i++)
      {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
      printf("\n");
  }
  mysql_free_result(result);
  mysql_close(con);
  exit(0);
}