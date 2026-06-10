
#include <stdio.h>

#define ROWS 20
#define COLS 50

char picture[ROWS][COLS];


void clearPicture()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            picture[i][j] = '_';
        }
    }
}


void displayPicture()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            printf("%c", picture[i][j]);
        }
        printf("\n");
    }
}


void drawRectangle(int row, int col, int height, int width)
{
    int i, j;

    for(i = row; i < row + height; i++)
    {
        for(j = col; j < col + width; j++)
        {
            if(i < ROWS && j < COLS)
            {
                picture[i][j] = '*';
            }
        }
    }
}

int main()
{
    clearPicture();

    drawRectangle(5, 10, 4, 8);

    printf("2D Graphics Editor\n\n");

    displayPicture();

    return 0;
}
// Draw line
void drawLine(int row1, int col1, int row2, int col2)
{
    int i;

    // Horizontal line
    if(row1 == row2)
    {
        for(i = col1; i <= col2; i++)
        {
            picture[row1][i] = '*';
        }
    }

    // Vertical line
    else if(col1 == col2)
    {
        for(i = row1; i <= row2; i++)
        {
            picture[i][col1] = '*';
        }
    }
}