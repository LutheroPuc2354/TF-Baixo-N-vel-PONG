#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>

const int LARGURA = 800;
const int ALTURA = 600;
const int TAM_RAQUETE = 100;
const int LARG_RAQUETE = 20;
const int TAM_BOLA = 20;

typedef enum {
    SELECIONANDO_MODO,
    SELECIONANDO_DIFICULDADE,
    JOGANDO,
    FIM_DE_JOGO
} EstadoJogo;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    srand(time(NULL));

    SDL_Window* win = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, 0);
    SDL_Renderer* render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* imgFundo = IMG_Load("fundoPong.jpg");
    SDL_Texture* fundo = SDL_CreateTextureFromSurface(render, imgFundo);
    SDL_FreeSurface(imgFundo);

    TTF_Font* fonte = TTF_OpenFont("fontePong.ttf", 32);
    SDL_Color branco = {255, 255, 255, 255};

    SDL_Rect player = {50, ALTURA / 2 - TAM_RAQUETE / 2, LARG_RAQUETE, TAM_RAQUETE};
    SDL_Rect cpu = {LARGURA - 70, ALTURA / 2 - TAM_RAQUETE / 2, LARG_RAQUETE, TAM_RAQUETE};
    SDL_Rect bola = {LARGURA / 2, ALTURA / 2, TAM_BOLA, TAM_BOLA};

    SDL_Rect botaoPvP = {LARGURA/2 - 150, ALTURA/2 - 40, 120, 50};
    SDL_Rect botaoCPU = {LARGURA/2 + 30, ALTURA/2 - 40, 120, 50};
    SDL_Rect botaoJogarNovamente = { LARGURA/2 - 100, ALTURA/2 + 50, 200, 50 }; //altultima


    SDL_Rect botaoDif1 = {LARGURA/2 - 150, ALTURA/2 + 40, 120, 50};
    SDL_Rect botaoDif2 = {LARGURA/2 + 30, ALTURA/2 + 40, 120, 50};

    int velX = 5;
    int velY = 5;
    int pontosPlayer = 0;
    int pontosCPU = 0;
    bool modoCPU = true;
    int dificuldade = 1;
    EstadoJogo estado = SELECIONANDO_MODO;

    SDL_Event e;

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return 0;

            if (estado == SELECIONANDO_MODO && e.type == SDL_MOUSEBUTTONDOWN) {
    int x = e.button.x;
    int y = e.button.y;
    if (x >= botaoPvP.x && x <= botaoPvP.x + botaoPvP.w &&
        y >= botaoPvP.y && y <= botaoPvP.y + botaoPvP.h) {
        modoCPU = false;
        estado = SELECIONANDO_DIFICULDADE;  // Ir para escolha de dificuldade
    }
    if (x >= botaoCPU.x && x <= botaoCPU.x + botaoCPU.w &&
        y >= botaoCPU.y && y <= botaoCPU.y + botaoCPU.h) {
        modoCPU = true;
        estado = SELECIONANDO_DIFICULDADE;  // Ir para escolha de dificuldade
    }
}


            if (estado == SELECIONANDO_DIFICULDADE && e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= botaoDif1.x && x <= botaoDif1.x + botaoDif1.w &&
                    y >= botaoDif1.y && y <= botaoDif1.y + botaoDif1.h) {
                    dificuldade = 1;
                    velX = 5;
                    velY = 5;
                    estado = JOGANDO;
                }
                if (x >= botaoDif2.x && x <= botaoDif2.x + botaoDif2.w &&
                    y >= botaoDif2.y && y <= botaoDif2.y + botaoDif2.h) {
                    dificuldade = 2;
                    velX = 8;
                    velY = 8;
                    estado = JOGANDO;
                }
            }

           if (estado == FIM_DE_JOGO && e.type == SDL_MOUSEBUTTONDOWN) { //altultima
    int x = e.button.x;
    int y = e.button.y;
    if (x >= botaoJogarNovamente.x && x <= botaoJogarNovamente.x + botaoJogarNovamente.w &&
        y >= botaoJogarNovamente.y && y <= botaoJogarNovamente.y + botaoJogarNovamente.h) {
        estado = SELECIONANDO_MODO;
        pontosPlayer = 0;
        pontosCPU = 0;
        bola.x = LARGURA / 2;
        bola.y = ALTURA / 2;
    }
}

        }

        if (estado == JOGANDO) {
            const Uint8* teclas = SDL_GetKeyboardState(NULL);
            if (teclas[SDL_SCANCODE_W] && player.y > 0)
                player.y -= 10;
            if (teclas[SDL_SCANCODE_S] && player.y + TAM_RAQUETE < ALTURA)
                player.y += 10;

            if (!modoCPU) {
                if (teclas[SDL_SCANCODE_UP] && cpu.y > 0)
                    cpu.y -= 10;
                if (teclas[SDL_SCANCODE_DOWN] && cpu.y + TAM_RAQUETE < ALTURA)
                    cpu.y += 10;
            } else {
        // Movimento CPU com leve erro para não ser perfeita
        int erro = rand() % 20 - 10; // -10 a +10 de erro
        if (bola.y + TAM_BOLA/2 < cpu.y + TAM_RAQUETE/2 + erro)
            cpu.y -= 6;
        if (bola.y + TAM_BOLA/2 > cpu.y + TAM_RAQUETE/2 + erro)
            cpu.y += 6;
        }


            bola.x += velX;
            bola.y += velY;

            if (bola.y <= 0 || bola.y + TAM_BOLA >= ALTURA)
                velY = -velY;

            if (SDL_HasIntersection(&bola, &player) || SDL_HasIntersection(&bola, &cpu))
                velX = -velX;

            if (bola.x < 0) {
                pontosCPU++;
                bola.x = LARGURA / 2;
                bola.y = ALTURA / 2;
            }

            if (bola.x > LARGURA) {
                pontosPlayer++;
                bola.x = LARGURA / 2;
                bola.y = ALTURA / 2;
            }

            if (pontosPlayer >= 5 || pontosCPU >= 5)
                estado = FIM_DE_JOGO;
        }

        // Desenho
        SDL_RenderClear(render);
        SDL_RenderCopy(render, fundo, NULL, NULL);

        if (estado == SELECIONANDO_MODO) {
            SDL_SetRenderDrawColor(render, 0, 128, 0, 255);
            SDL_RenderFillRect(render, &botaoPvP);
            SDL_RenderFillRect(render, &botaoCPU);

            SDL_Surface* s1 = TTF_RenderText_Solid(fonte, "PvP", branco);
            SDL_Surface* s2 = TTF_RenderText_Solid(fonte, "CPU", branco);
            SDL_Texture* t1 = SDL_CreateTextureFromSurface(render, s1);
            SDL_Texture* t2 = SDL_CreateTextureFromSurface(render, s2);
            SDL_Rect r1 = {botaoPvP.x + 30, botaoPvP.y + 10, s1->w, s1->h};
            SDL_Rect r2 = {botaoCPU.x + 20, botaoCPU.y + 10, s2->w, s2->h};
            SDL_RenderCopy(render, t1, NULL, &r1);
            SDL_RenderCopy(render, t2, NULL, &r2);
            SDL_FreeSurface(s1); SDL_FreeSurface(s2);
            SDL_DestroyTexture(t1); SDL_DestroyTexture(t2);
        }

        if (estado == SELECIONANDO_DIFICULDADE) {
    // Renderiza textos para saber o tamanho exato
    SDL_Surface* s1 = TTF_RenderText_Solid(fonte, "Dificuldade 1", branco);
    SDL_Surface* s2 = TTF_RenderText_Solid(fonte, "Dificuldade 2", branco);

    // Define padding
    int paddingX = 20;
    int paddingY = 20;

    // Ajusta botões para caber o texto + padding
    botaoDif1.w = s1->w + paddingX;
    botaoDif1.h = s1->h + paddingY;
    botaoDif2.w = s2->w + paddingX;
    botaoDif2.h = s2->h + paddingY;

    // Centraliza na tela novamente
    botaoDif1.x = LARGURA/2 - botaoDif1.w - 20;
    botaoDif1.y = ALTURA/2 - botaoDif1.h/2;
    botaoDif2.x = LARGURA/2 + 20;
    botaoDif2.y = ALTURA/2 - botaoDif2.h/2;

    // Desenha os fundos corrigidos
    SDL_SetRenderDrawColor(render, 128, 0, 128, 255);
    SDL_RenderFillRect(render, &botaoDif1);
    SDL_RenderFillRect(render, &botaoDif2);

    // Cria texturas e renderiza texto centralizado
    SDL_Texture* t1 = SDL_CreateTextureFromSurface(render, s1);
    SDL_Texture* t2 = SDL_CreateTextureFromSurface(render, s2);

    SDL_Rect r1 = {
        botaoDif1.x + (botaoDif1.w - s1->w)/2,
        botaoDif1.y + (botaoDif1.h - s1->h)/2,
        s1->w, s1->h
    };
    SDL_Rect r2 = {
        botaoDif2.x + (botaoDif2.w - s2->w)/2,
        botaoDif2.y + (botaoDif2.h - s2->h)/2,
        s2->w, s2->h
    };

    SDL_RenderCopy(render, t1, NULL, &r1);
    SDL_RenderCopy(render, t2, NULL, &r2);

    SDL_FreeSurface(s1); SDL_FreeSurface(s2);
    SDL_DestroyTexture(t1); SDL_DestroyTexture(t2);
}


        if (estado == JOGANDO || estado == FIM_DE_JOGO) {
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            SDL_RenderFillRect(render, &player);
            SDL_RenderFillRect(render, &cpu);
            SDL_RenderFillRect(render, &bola);

            char texto[100];
            if (modoCPU)
                sprintf(texto, "Jogador: %d  CPU: %d", pontosPlayer, pontosCPU);
            else
                sprintf(texto, "Jogador 1: %d  Jogador 2: %d", pontosPlayer, pontosCPU);

            SDL_Surface* s = TTF_RenderText_Solid(fonte, texto, branco);
            SDL_Texture* t = SDL_CreateTextureFromSurface(render, s);
            SDL_Rect r = {LARGURA/2 - s->w/2, 20, s->w, s->h};
            SDL_RenderCopy(render, t, NULL, &r);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }

        if (estado == FIM_DE_JOGO) {
    const char* msg;
    if (modoCPU)
        msg = (pontosPlayer > pontosCPU) ? "PLAYER VENCEU!" : "CPU VENCEU!";
    else
        msg = (pontosPlayer > pontosCPU) ? "PLAYER 1 VENCEU!" : "PLAYER 2 VENCEU!";

    SDL_Surface* s = TTF_RenderText_Solid(fonte, msg, branco);
    SDL_Texture* t = SDL_CreateTextureFromSurface(render, s);

    // Desenha fundo preto atrás do texto
    SDL_Rect r = {LARGURA/2 - s->w/2 - 20, ALTURA/2 - s->h/2 - 10, s->w + 40, s->h + 20};
    SDL_SetRenderDrawColor(render, 0, 0, 0, 200); // Preto semi-transparente
    SDL_RenderFillRect(render, &r);

    // Agora desenha o texto por cima
    SDL_Rect textoR = {LARGURA/2 - s->w/2, ALTURA/2 - s->h/2, s->w, s->h};
    SDL_RenderCopy(render, t, NULL, &textoR);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);

    // Desenha botão "JOGAR NOVAMENTE"
SDL_SetRenderDrawColor(render, 0, 128, 0, 255);
SDL_RenderFillRect(render, &botaoJogarNovamente);

SDL_Surface* sBtn = TTF_RenderText_Solid(fonte, "JOGAR NOVAMENTE", branco);
SDL_Texture* tBtn = SDL_CreateTextureFromSurface(render, sBtn);
SDL_Rect rBtn = {
    botaoJogarNovamente.x + (botaoJogarNovamente.w - sBtn->w)/2,
    botaoJogarNovamente.y + (botaoJogarNovamente.h - sBtn->h)/2,
    sBtn->w, sBtn->h
};
SDL_RenderCopy(render, tBtn, NULL, &rBtn);
SDL_FreeSurface(sBtn);
SDL_DestroyTexture(tBtn);

}


        SDL_RenderPresent(render);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}