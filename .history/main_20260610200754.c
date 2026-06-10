
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 50
#define MAX_OBJECTS 100

#define EMPTY '_'
#define PIXEL '*'

typedef enum
{
    RECTANGLE = 1,
    LINE,
    TRIANGLE,
    CIRCLE
} ShapeType;

typedef struct
{
    int id;
    int active;
    ShapeType type;

    int row1;
    int col1;
    int row2;
    int col2;

    int height;
    int width;
    int radius;

} Object;

char picture[ROWS][COLS];
Object objects[MAX_OBJECTS];

int objectCount = 0;
int nextId = 1;

/* ---------- Utility Functions ---------- */

int isValidPosition(int row, int col)
{
    return row >= 0 &&
           row < ROWS &&
           col >= 0 &&
           col < COLS;
}

void setPixel(int row, int col, char value)
{
    if (isValidPosition(row, col))
    {
        picture[row][col] = value;
    }
}

void clearPicture(void)
{
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            picture[i][j] = EMPTY;
        }
    }
}

void displayPicture(void)
{
    int i, j;

    printf("\n");

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            printf("%c ", picture[i][j]);
        }
        printf("\n");
    }
}

/* ---------- Drawing Functions ---------- */

void drawRectangle(int row,
                   int col,
                   int height,
                   int width)
{
    int i, j;

    for (i = row; i < row + height; i++)
    {
        for (j = col; j < col + width; j++)
        {
            setPixel(i, j, PIXEL);
        }
    }
}

/* Bresenham Line Algorithm */
void drawLine(int row1,
              int col1,
              int row2,
              int col2)
{
    int dx = abs(col2 - col1);
    int dy = abs(row2 - row1);

    int sx = (col1 < col2) ? 1 : -1;
    int sy = (row1 < row2) ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
        setPixel(row1, col1, PIXEL);

        if (row1 == row2 &&
            col1 == col2)
        {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            col1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            row1 += sy;
        }
    }
}

void drawTriangle(int row,
                  int col,
                  int height)
{
    int i, j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j <= i; j++)
        {
            setPixel(row + i,
                     col + j,
                     PIXEL);
        }
    }
}

void drawCircle(int centerRow,
                int centerCol,
                int radius)
{
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            int dx = j - centerCol;
            int dy = i - centerRow;

            if ((dx * dx + dy * dy)
                <= radius * radius)
            {
                setPixel(i, j, PIXEL);
            }
        }
    }
}

/* ---------- Canvas Redraw ---------- */

void redrawCanvas(void)
{
    int i;

    clearPicture();

    for (i = 0; i < objectCount; i++)
    {
        if (!objects[i].active)
        {
            continue;
        }

        switch (objects[i].type)
        {
            case RECTANGLE:
                drawRectangle(
                    objects[i].row1,
                    objects[i].col1,
                    objects[i].height,
                    objects[i].width
                );
                break;

            case LINE:
                drawLine(
                    objects[i].row1,
                    objects[i].col1,
                    objects[i].row2,
                    objects[i].col2
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    objects[i].row1,
                    objects[i].col1,
                    objects[i].height
                );
                break;

            case CIRCLE:
                drawCircle(
                    objects[i].row1,
                    objects[i].col1,
                    objects[i].radius
                );
                break;
        }
    }
}

/* ---------- Object Management ---------- */

void addObject(void)
{
    Object obj;

    if (objectCount >= MAX_OBJECTS)
    {
        printf("Object limit reached!\n");
        return;
    }

    obj.id = nextId++;
    obj.active = 1;

    printf("\nObject Type\n");
    printf("1. Rectangle\n");
    printf("2. Line\n");
    printf("3. Triangle\n");
    printf("4. Circle\n");

    printf("Choose: ");
    scanf("%d", (int *)&obj.type);

    switch (obj.type)
    {
        case RECTANGLE:
            printf("Row Col Height Width: ");
            scanf("%d %d %d %d",
                  &obj.row1,
                  &obj.col1,
                  &obj.height,
                  &obj.width);
            break;

        case LINE:
            printf("Row1 Col1 Row2 Col2: ");
            scanf("%d %d %d %d",
                  &obj.row1,
                  &obj.col1,
                  &obj.row2,
                  &obj.col2);
            break;

        case TRIANGLE:
            printf("Row Col Height: ");
            scanf("%d %d %d",
                  &obj.row1,
                  &obj.col1,
                  &obj.height);
            break;

        case CIRCLE:
            printf("CenterRow CenterCol Radius: ");
            scanf("%d %d %d",
                  &obj.row1,
                  &obj.col1,
                  &obj.radius);
            break;

        default:
            printf("Invalid type!\n");
            return;
    }

    objects[objectCount++] = obj;

    redrawCanvas();

    printf("Object added. ID = %d\n",
           obj.id);
}

void deleteObject(void)
{
    int id, i;

    printf("Enter Object ID: ");
    scanf("%d", &id);

    for (i = 0; i < objectCount; i++)
    {
        if (objects[i].id == id &&
            objects[i].active)
        {
            objects[i].active = 0;

            redrawCanvas();

            printf("Deleted.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

void modifyObject(void)
{
    int id, i;

    printf("Enter Object ID: ");
    scanf("%d", &id);

    for (i = 0; i < objectCount; i++)
    {
        if (objects[i].id == id &&
            objects[i].active)
        {
            Object *obj = &objects[i];

            switch (obj->type)
            {
                case RECTANGLE:
                    printf("New Row Col Height Width: ");
                    scanf("%d %d %d %d",
                          &obj->row1,
                          &obj->col1,
                          &obj->height,
                          &obj->width);
                    break;

                case LINE:
                    printf("New Row1 Col1 Row2 Col2: ");
                    scanf("%d %d %d %d",
                          &obj->row1,
                          &obj->col1,
                          &obj->row2,
                          &obj->col2);
                    break;

                case TRIANGLE:
                    printf("New Row Col Height: ");
                    scanf("%d %d %d",
                          &obj->row1,
                          &obj->col1,
                          &obj->height);
                    break;

                case CIRCLE:
                    printf("New CenterRow CenterCol Radius: ");
                    scanf("%d %d %d",
                          &obj->row1,
                          &obj->col1,
                          &obj->radius);
                    break;
            }

            redrawCanvas();

            printf("Modified.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

void listObjects(void)
{
    int i;

    printf("\nObjects:\n");

    for (i = 0; i < objectCount; i++)
    {
        if (objects[i].active)
        {
            printf("ID: %d | ",
                   objects[i].id);

            switch(objects[i].type)
            {
                case RECTANGLE:
                    printf("Rectangle");
                    break;

                case LINE:
                    printf("Line");
                    break;

                case TRIANGLE:
                    printf("Triangle");
                    break;

                case CIRCLE:
                    printf("Circle");
                    break;
            }

            printf("\n");
        }
    }
}

/* ---------- Main ---------- */

int main(void)
{
    int choice;

    clearPicture();

    do
    {
        printf("\n===== 2D Graphics Editor =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayPicture();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}
