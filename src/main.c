# include <gtk/gtk.h>
# include <stdio.h>


//typedef struct {
//    int row;
//    int col;
//} ButtonPosition;
//ButtonPosition btnCords[9];

typedef struct {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
} widgets_t;

typedef enum {
    EMPTY,
    PLAYER_X,
    PLAYER_O,
} Option;

typedef struct {
    int moves;
    int rounds;
    Option current_player;
    Option board[9];
} GameInfo;
GameInfo game;

void on_main_window_destroy (void) {
    gtk_main_quit();
}

void initialize_game() {
    memset(&game, 0, sizeof(GameInfo));  // Define todos os atributos de game como 0
    game.current_player = PLAYER_X;  // Define o jogador inicial
}

Option check_winner(const Option board[9]) {

    int rules[9][3] = {
            {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
            {1, 4, 7}, { 2, 5, 8 }, { 3, 6, 9 },
            {1, 5, 9}, {3, 5, 7}
    };

    for (int pl = 0; pl < 2; pl++) {  // Percorre os jogadores
        for(int i = 0; i < 8; i++){  // Percorre as regras
            Option player = pl == 0 ? PLAYER_X : PLAYER_O;
            int player_won = 1;
            for (int x = 0; x < 3; x++){  // Percorre as posições
                if (board[rules[i][x]-1] != player) {
                    player_won = 0;
                    break;
                }
            }
            if (player_won) {
                return player;
            }
        }
    }

    return EMPTY;
}

int main (int argc, char *argv[]) {
    initialize_game();

    // Inicializa variável builder
    GtkBuilder *builder;

    widgets_t *widgets = g_slice_new(widgets_t);

    gtk_init(&argc, &argv);

    // Cria builder
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/old-woman-game.glade", NULL);

    widgets->window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    widgets->grid = GTK_WIDGET(gtk_builder_get_object(builder, "board"));
    widgets->label = GTK_WIDGET(gtk_builder_get_object(builder, "player_turn"));

    gtk_builder_connect_signals(builder, widgets);

    // Apaga o builder
    g_object_unref(builder);

    // Initializa a janelawidgets
    gtk_widget_show (widgets->window);
    gtk_main();

    // Apaga o objeto widgets
    g_slice_free (widgets_t, widgets);

    return 0;
}

void onClickOption(GtkWidget *widget, GdkEventKey *event, void *data) {
    widgets_t *widgets = (widgets_t *)data;

    GtkWidget *button = GTK_WIDGET(widget);

    int btnIndex = atoi(gtk_widget_get_name(button));

    if (game.board[btnIndex] != EMPTY) {
        return;
    }

    game.moves ++;
    game.board[btnIndex] = game.current_player;

    if (game.current_player == PLAYER_X) {
        gtk_button_set_label(GTK_BUTTON(button), "X");
    } else {
        gtk_button_set_label(GTK_BUTTON(button), "O");
    }

    Option winner = check_winner(game.board);
    if (winner) {
        GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (widgets->window),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Vencedor: Jogador %c",
                                                    winner == PLAYER_X ? 'X' : 'O');
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
        on_main_window_destroy();

        printf("O vencedor foi o %c", winner == PLAYER_X ? 'X' : 'O');
    } else if (game.moves == 9) {
        GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (widgets->window),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Deu velha");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
        on_main_window_destroy();

    }

    // Alterna o jogador
    game.current_player = (game.current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    if (game.current_player == PLAYER_X) {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Vez do Jogador: X");
    } else {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Vez do Jogador: O");
    }
}