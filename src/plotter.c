#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"

#define MAX_TREES (2 << 4)

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    NodeTree *trees[MAX_TREES];
    size_t count;
} TreesBuffer; 

Color graph_colors[] = { RED, GREEN, PURPLE };
const size_t graph_colors_count = sizeof(graph_colors) / sizeof(graph_colors[0]);

Vector2 screen(float x, float y, int scale)
{
    return (Vector2) {
        .x = (x + scale)/(2*scale) * WIDTH,
        .y = (1 - (y + scale)/(2*scale)) * HEIGHT
    };
}

TreesBuffer parse_input_trees(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s expressions\n", argv[0]);
        exit(1);
    }
    TreesBuffer res = {0};
    char expression[256];
    for (int i = 1; i < argc; i++) {
        strncpy(expression, argv[i], sizeof(expression));
        NodeTree *tree = tree_parse(expression);
        if (tree == NULL) {
            fprintf(stderr, "Duck\n");
            exit(EXIT_FAILURE);
        }
        res.trees[i-1] = tree;
    }
    res.count = argc - 1;
    return res;
}

int main(int argc, char *argv[])
{
    TreesBuffer trees_buf = parse_input_trees(argc, argv);

    InitWindow(WIDTH, HEIGHT, "Decmoc");
    SetTargetFPS(5);

    RenderTexture2D texture = LoadRenderTexture(WIDTH, HEIGHT);
    
    BeginTextureMode(texture);
    
    DrawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, RAYWHITE);
    DrawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, RAYWHITE);
    
    EndTextureMode();

    int scale = 1;
    float wheel;
    while (WindowShouldClose() == 0) {
        //if (GetKeyPressed() == KEY_ENTER) {
        //    printf("f(x): ");
        //    fgets(expression, sizeof(expression), stdin);
        //    expression[strcspn(expression, "\n")] = '\0';
        //    printf("%s\n", expression);
        //    tree = tree_parse(expression);
        //    ClearBackground(BLACK);
        //}

        BeginDrawing();

        DrawTextureRec(texture.texture, (Rectangle){ 0, 0, WIDTH, HEIGHT }, (Vector2){ 0, 0 }, WHITE);

        if (wheel = GetMouseWheelMove()) {
            scale += -wheel;
            ClearBackground(BLACK);
        }

        // Draw the graphs
        for (size_t i = 0; i < trees_buf.count; i++) {
            for (float x = -scale; x <= scale; x += 0.005) {
                float y = tree_eval(trees_buf.trees[i], x);
                Color color = graph_colors[i % graph_colors_count];
                DrawPixelV(screen(x, y, scale), color);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
