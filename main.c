#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

typedef struct
{
    float x;
    float y;
    float width;
    float height;
    float speed;
    float score;
} Paddle;
typedef struct
{
    float x;
    float vx;
    float y;
    float vy;
    float r;
    float speed;
} Ball;

bool collideWPlayer(Paddle, Ball);

int main()
{
    // Janela
    const int window_width = 1080;
    const int window_height = 720;
    InitWindow(window_width, window_height, "janela");

    // Jogadores 1 e 2
    Paddle paddle1 = {5, window_height/2, 20, 100, 300, 0};
    Paddle paddle2 = {window_width-25, window_height/2, 20, 100, 300, 0};

    char p1scoreSTR[200];
    char p2scoreSTR[200];

    // Bola
    Ball ball = {window_width/2, 1, window_height/2, 1, 10, 400};
    
    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Movimento 1 Jogador
        if(IsKeyDown(KEY_W)) paddle1.y -= paddle1.speed * dt;
        if(IsKeyDown(KEY_S)) paddle1.y += paddle1.speed * dt;

        // Movimento 2 Jogador
        if(IsKeyDown(KEY_UP)) paddle2.y -= paddle2.speed * dt;
        if(IsKeyDown(KEY_DOWN)) paddle2.y += paddle2.speed * dt;

        // Colisões com os Cantos JOGADORES
        if(paddle1.y <= 0) paddle1.y = 0;
        if(paddle1.y + paddle1.height >= window_height) paddle1.y = window_height - paddle1.height;
        if(paddle2.y <= 0) paddle2.y = 0;
        if(paddle2.y + paddle2.height >= window_height) paddle2.y = window_height - paddle2.height;

        // Movimento da Bola
        ball.x += (ball.speed * dt) * ball.vx;
        ball.y += (ball.speed * dt) * ball.vy;

        // Colisão com os Cantos BOLA e Pontuação
        if(ball.x <= 0) ball.vx = 1, paddle2.score++, ball.x = window_width/2-ball.r/2, ball.y = window_height/2-ball.r/2;
        if(ball.x + ball.r >= window_width) ball.vx = -1, paddle1.score++, ball.x = window_width/2-ball.r/2, ball.y = window_height/2-ball.r/2;
        if(ball.y <= 0) ball.vy = 1;
        if(ball.y + ball.r >= window_height) ball.vy = -1;

        // Colisão Com os Jogadores BOLA
        if (collideWPlayer(paddle1, ball)) {
            ball.vx = 1;
            
            if (ball.y + ball.r / 2 < paddle1.y+paddle1.height/2) {
                ball.vy = -1;
            }
            else if(ball.y + ball.r / 2 > paddle1.y+paddle1.height/2) {
                ball.vy = 1;
            }
        }
        if (collideWPlayer(paddle2, ball)) {
            ball.vx = -1;
            
            if (ball.y + ball.r / 2 < paddle2.y+paddle2.height/2) {
                ball.vy = -1;
            }
            else if(ball.y + ball.r / 2 > paddle2.y+paddle2.height/2) {
                ball.vy = 1;
            }
        }


        sprintf(p1scoreSTR, "%.2f", paddle1.score);
        sprintf(p2scoreSTR, "%.2f", paddle2.score);

        // Desenha
        BeginDrawing();
            
            ClearBackground(DARKGRAY);

            // 1 Jogador
            DrawRectangle(paddle1.x, paddle1.y, paddle1.width, paddle1.height, BLUE);

            // 2 Jogador
            DrawRectangle(paddle2.x, paddle2.y, paddle2.width, paddle2.height, BLUE);

            // Bola
            DrawCircle(ball.x+ball.r/2, ball.y+ball.r/2, ball.r, RED);

            // Desenha a Pontuação
            DrawText(p1scoreSTR, window_width/2 - 200, 10, 24, WHITE);
            DrawText(p2scoreSTR, window_width/2 + 200, 10, 24, WHITE);

            // Linha no Meio
            //DrawRectangle(window_width/2-20/2, 0, 20, window_height, WHITE);
            // w= 20 h= 50
            for(int i = 0; i <= window_height/(20 + 10); i++)
            {
                int posY = i*(20 + 10) + 5;
                DrawRectangle(window_width / 2 -5, posY, 10, 20, WHITE);
            }


        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}

bool collideWPlayer(Paddle paddle, Ball ball) {
    // Procura o ponto mais perto do jogador para o da bola
    float closestX = fmax(paddle.x, fmin(ball.x, paddle.x + paddle.width));
    float closestY = fmax(paddle.y, fmin(ball.y, paddle.y + paddle.height));

    // Computa a distancia do centro da bola para o ponto mais proximo
    float distanceX = ball.x - closestX;
    float distanceY = ball.y - closestY;
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    
    // Se a distancia e menor ou igual ao raio da bola, tem uma colisão
    return distanceSquared <= (ball.r * ball.r);
}
