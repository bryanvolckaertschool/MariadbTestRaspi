#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <mysql.h>

#ifndef CONSUMER
#define CONSUMER "Consumer"
#endif

void finish_with_error(MYSQL *con)
{
    printf("Finished with error. \n");
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char **argv)
{

    char querry[80];
    char *chipname = "gpiochip0";
    unsigned int line_num = 27;
    int val;
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int i, ret, prev_value;

    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "pi", "raspberry", "io_log", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    chip = gpiod_chip_open_by_name(chipname);
    if (!chip)
    {
        perror("Open chip failed\n");
        goto end;
    }

    line = gpiod_chip_get_line(chip, line_num);
    if (!line)
    {
        perror("Get line failed\n");
        goto close_chip;
    }

    ret = gpiod_line_request_input(line, CONSUMER);
    if (ret < 0)
    {
        perror("Request line as input failed\n");
        goto release_line;
    }

    while (1)
    {
        val = gpiod_line_get_value(line);
        if (val < 0)
        {
            perror("Read line input failed\n");
            goto release_line;
        }
        else if (val != prev_value)
        {
            printf("change detected\n");

            sprintf(querry, "INSERT INTO `IO`(`gpio_number`, `state`, `date`) VALUES (%d,%d,now())", line_num, val);
            printf("%s\n", querry);

            if (mysql_query(con, querry))
                finish_with_error(con);
        }

        prev_value = val;
    }

release_line:
    gpiod_line_release(line);
close_chip:
    gpiod_chip_close(chip);
end:
    return 0;
}