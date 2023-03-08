use core::panic;
use rand::Rng;
use std::io::stdin;

use std::vec as arr;
type Arr<T> = Vec<T>;
macro_rules! flush {
    // <- copypasted the `say_hello` macro example (in Rust by Example) and slightly modified it, no understanding of Rust macro creation yet.
    () => {
        use std::io::{stdout, Write};
        stdout()
            .flush()
            .expect("Called 'flush!()' and failed to flush the output stream");
    };
} // I use these to get think about the code and syntax more easlily since I'm not familiar yet

#[derive(PartialEq)] // <-- suggested by 'rust-analyzer', what does this do? I understand it makes the '==' operator work for structs / enums but no more than that
enum Turn {
    Even,
    Odd,
}

impl Turn {
    fn flip(self) -> Self {
        match self {
            Self::Even => Self::Odd,
            Self::Odd => Self::Even,
        }
    }
}

#[derive(Clone)]
// <-- suggested by 'rust-analyzer', what does this do? I understand it adds '.clone()' but not more than that
#[derive(PartialEq)]
enum Cell {
    Empty,
    PlayerOne,
    PlayerTwo,
}

#[derive(PartialEq)]
enum GameState {
    Uninititaed,
    Ongoing,
    TrisPlayer(u8),
    TrisCom(u8),
    Tie,
}

#[derive(Clone)]
struct Point(usize, usize);

type Board = Arr<Arr<Cell>>;

type Chart = Arr<Arr<u32>>;

struct GameBoard {
    content: Board,
    symbols: (char, char),
}

impl GameBoard {
    fn new(board_width: &usize, board_heigth: &usize, fill_value: Cell) -> GameBoard {
        GameBoard {
            content: arr![arr![fill_value; *board_width]; *board_heigth],
            symbols: ('X', 'O'),
        }
    }

    fn new_empty(board_width: &usize, board_heigth: &usize) -> GameBoard {
        Self::new(board_width, board_heigth, Cell::Empty)
    }

    fn display(&self) -> () {
        // I would've done these using 'for r in self.content' then 'for c in r' but I don't have '.begin()' and '.end()' here
        for r in 0..self.content.len() {
            for c in 0..self.content[0].len() {
                match self
                    .content
                    .get(self.content.len() - r - 1)
                    .unwrap()
                    .get(c)
                    .unwrap()
                {
                    Cell::Empty => print!(" "),
                    Cell::PlayerOne => print!("{}", self.symbols.0),
                    Cell::PlayerTwo => print!("{}", self.symbols.1),
                }
                if c < self.content.len() - 1 {
                    print!(" │ ")
                }
            }
            if r != self.content.len() - 1 {
                print!("\n─");
                for _ in 0..self.content.len() - 1 {
                    print!("─┼──");
                }
                print!("\n");
            }
        }
        flush!();
    }

    fn get_state(&self, tris_size: &usize, current_turn: &Turn, is_com_turn: &bool) -> GameState {
        for r in 0..self.content.len() {
            for c in 0..self.content[0].len() {
                let mut tris_parts = 0;
                for i in 1..*tris_size {
                    if true
                        && r <= self.content.len() - tris_size
                        && self.content[r + (i - 1)][c + (0 - 0)] != Cell::Empty
                        && self.content[r + (i - 1)][c + (0 - 0)] == self.content[r + i][c + 0]
                    {
                        tris_parts += 1;
                        if tris_parts == tris_size - 1 {
                            if cfg!(debug_assertions) {
                                println!("\x1b[96;1mDEBUG\x1b[0m: TRIS - 90°[ {r}:{c} ]")
                            }
                            return if *is_com_turn {
                                GameState::TrisCom(if *current_turn == Turn::Even { 1 } else { 2 })
                            } else {
                                GameState::TrisPlayer(if *current_turn == Turn::Even {
                                    1
                                } else {
                                    2
                                })
                            };
                        }
                    }
                }
                tris_parts = 0;
                for i in 1..*tris_size {
                    if true
                        && c <= self.content[0].len() - tris_size
                        && self.content[r + (0 - 0)][c + (i - 1)] != Cell::Empty
                        && self.content[r + (0 - 0)][c + (i - 1)] == self.content[r + 0][c + i]
                    {
                        tris_parts += 1;
                        if tris_parts == tris_size - 1 {
                            if cfg!(debug_assertions) {
                                println!("\x1b[96;1mDEBUG\x1b[0m: TRIS - 0°[ {r}:{c} ]")
                            }
                            return if *is_com_turn {
                                GameState::TrisCom(if *current_turn == Turn::Even { 1 } else { 2 })
                            } else {
                                GameState::TrisPlayer(if *current_turn == Turn::Even {
                                    1
                                } else {
                                    2
                                })
                            };
                        }
                    }
                }
                tris_parts = 0;
                for i in 1..*tris_size {
                    if true
                        && r <= self.content.len() - tris_size
                        && c <= self.content[0].len() - tris_size
                        && self.content[r + (i - 1)][c + (i - 1)] != Cell::Empty
                        && self.content[r + (i - 1)][c + (i - 1)] == self.content[r + i][c + i]
                    {
                        tris_parts += 1;
                        if tris_parts == tris_size - 1 {
                            if cfg!(debug_assertions) {
                                println!("\x1b[96;1mDEBUG\x1b[0m: TRIS - 45°[ {r}:{c} ]")
                            }
                            return if *is_com_turn {
                                GameState::TrisCom(if *current_turn == Turn::Even { 1 } else { 2 })
                            } else {
                                GameState::TrisPlayer(if *current_turn == Turn::Even {
                                    1
                                } else {
                                    2
                                })
                            };
                        }
                    }
                }
                tris_parts = 0;
                for i in 1..*tris_size {
                    if true
                        && r <= self.content.len() - tris_size
                        && c >= tris_size - 1
                        && self.content[r + (i - 1)][c - (i - 1)] != Cell::Empty
                        && self.content[r + (i - 1)][c - (i - 1)] == self.content[r + i][c - i]
                    {
                        tris_parts += 1;
                        if tris_parts == tris_size - 1 {
                            if cfg!(debug_assertions) {
                                println!("\x1b[96;1mDEBUG\x1b[0m: TRIS - 135°[ {r}:{c} ]")
                            }
                            return if *is_com_turn {
                                GameState::TrisCom(if *current_turn == Turn::Even { 1 } else { 2 })
                            } else {
                                GameState::TrisPlayer(if *current_turn == Turn::Even {
                                    1
                                } else {
                                    2
                                })
                            };
                        }
                    }
                }
            }
        }

        for r in &self.content {
            for c in r {
                if *c == Cell::Empty {
                    return GameState::Ongoing;
                }
            }
        }
        GameState::Tie
    }

    // due to the verbosity of the method I asked openai's chatbot to shorten it while maintining the same behaviour
    // I won't actually use it until I understand each difference between the two completely
    fn get_state_chatgpt(
        &self,
        tris_size: &usize,
        current_turn: &Turn,
        is_com_turn: &bool,
    ) -> GameState {
        let possible_directions = [(1, 0), (0, 1), (1, 1), (1, -1)];

        for &(r_dir, c_dir) in &possible_directions {
            for r in 0..self.content.len() {
                for c in 0..self.content[0].len() {
                    let mut tris_parts = 0;
                    for i in 1..*tris_size {
                        let r_new = r as i32 + r_dir * i as i32;
                        let c_new = c as i32 + c_dir * i as i32;
                        if r_new >= 0
                            && r_new < self.content.len() as i32
                            && c_new >= 0
                            && c_new < self.content[0].len() as i32
                            && self.content[r][c] != Cell::Empty
                            && self.content[r][c] == self.content[r_new as usize][c_new as usize]
                        {
                            tris_parts += 1;
                            if tris_parts == *tris_size - 1 {
                                let winner = if *is_com_turn {
                                    GameState::TrisCom(if *current_turn == Turn::Even {
                                        1
                                    } else {
                                        2
                                    })
                                } else {
                                    GameState::TrisPlayer(if *current_turn == Turn::Even {
                                        1
                                    } else {
                                        2
                                    })
                                };
                                if cfg!(debug_assertions) {
                                    let direction_name = match (r_dir, c_dir) {
                                        (1, 0) => "90°",
                                        (0, 1) => "0°",
                                        (1, 1) => "45°",
                                        (1, -1) => "135°",
                                        _ => unreachable!(),
                                    };
                                    println!("\x1b[96;1mDEBUG\x1b[0m: TRIS - {}[{}:{}]", direction_name, r, c);
                                }
                                return winner;
                            }
                        } else {
                            tris_parts = 0;
                        }
                    }
                }
            }
        }

        if self.content.iter().any(|row| row.contains(&Cell::Empty)) {
            GameState::Ongoing
        } else {
            GameState::Tie
        }
    }
}

fn main() -> Result<(), std::io::Error> {
    let command_line_args: Arr<String> = std::env::args().collect();

    // Prepare program
    let _game = GameState::Uninititaed; // for possible future debugging

    let mut history_flag = cfg!(debug_assertions);
    let mut com_opp_only = false;
    let mut com_opp_enabled = false;
    let mut have_delay = false;
    let /* mut */ com_opp_strength: u32 = 2;
    let mut tris_length: usize = 3;
    let /* mut */ board_heigth: usize = 7;
    let /* mut */ board_width: usize = 7;
    let /* mut */ player_symbols = ('X', 'O');

    // handle_command_line_args(command_line_args)?;
    // Ideas for the Ok() return type?
    // first idea was thinking of a tuple but that'd end up breaking the 12 value limit if I added any more options
    // Second idea is a "game_options" struct, but I don't think deconstructing it wouldn't be intuitive / redable

    let cla = command_line_args;
    for argv_index in 1..cla.len() {
        for arg_char in 1..cla[argv_index].len() {
            if cla[argv_index].get(arg_char - 1..=arg_char - 1).unwrap() == "-" {
                match cla[argv_index].get(arg_char..=arg_char).unwrap() {
                    "h" => {
                        history_flag = true;
                    }
                    "c" => {
                        com_opp_enabled = true;
                        match cla[argv_index]
                            .get(arg_char + 1..=arg_char + 1)
                            .unwrap_or(" ")
                        {
                            "d" => have_delay = true,
                            "o" => com_opp_only = true,
                            " " => {}
                            _ => unimplemented!(),
                        }
                    }
                    "t" => {
                        let tmp = cla[argv_index]
                            .get(arg_char + 1..=arg_char + 1)
                            .expect("missing number after '-t'")
                            .parse()
                            .expect("NaN after '-t'");

                        tris_length = if tmp < 3 {
                            panic!("Tris length length must be at least 3")
                        } else {
                            tmp
                        };
                    }
                    "s" => {
                        unimplemented!();
                    }
                    "b" => {
                        todo!();
                    }
                    v => {
                        println!("\x1b[93;1mwarning\x1b[0m: '-{}' is an invalid argument", v);
                    }
                }
            }
        }
    }

    if cfg!(debug_assertions) {
        print!(
            "    \x1b[96;1mDEBUG\x1b[0m: TRIS LENGTH - {tris_length}
    \x1b[96;1mDEBUG\x1b[0m: COM MAGNETISM - {com_opp_strength}
    \x1b[96;1mDEBUG\x1b[0m: BOARD WIDTH - {board_width}
    \x1b[96;1mDEBUG\x1b[0m: BOARD HEIGTH - {board_heigth}
"
        )
    }

    // Prepare Game
    let mut my_board = GameBoard::new_empty(&board_width, &board_heigth);
    my_board.symbols = player_symbols;
    let mut turn = Turn::Even;

    if tris_length > board_heigth || tris_length > board_width || tris_length < 3 {
        panic!(
            "Tris length ({tris_length}) should be shorter than the short side of the board ({})",
            if board_width >= board_heigth {
                board_heigth
            } else {
                board_width
            }
        );
    } // this will only matter when I manage to implement '-b'
    display_titlecard();
    flush!();

    // Game
    let game = loop {
        let is_com_turn = (com_opp_enabled && turn == Turn::Odd) || com_opp_only;

        my_board.display();

        display_turn(&turn, &is_com_turn, &Some(&my_board.symbols));

        if is_com_turn {
            if have_delay {
                std::thread::sleep(core::time::Duration::from_millis(500));
            }
            let com_move = com_opp_evaluate(&my_board.content, &turn, &com_opp_strength);
            com_control(&mut my_board.content, &turn, com_move);
        } else {
            player_control(&mut my_board.content, &turn);
        }

        manage_history(&board_width, &history_flag);

        let game = my_board.get_state(&tris_length, &turn, &is_com_turn);
        if game != GameState::Ongoing {
            break game;
        }

        turn = turn.flip();
    };
    my_board.display();
    display_exit_message(game);
    Ok(())
}

fn _handle_command_line_args(s: Arr<String>) -> Result<(), std::io::Error> {
    s.len();
    todo!()
}

fn display_titlecard() -> () {
    println!(
        r#"Welcome to 
    $$\               $$\           
    $$ |              \__|          
  $$$$$$\    $$$$$$\  $$\  $$$$$$$\ 
  \_$$  _|  $$  __$$\ $$ |$$  _____|
    $$ |    $$ |  \__|$$ |\$$$$$$\  
    $$ |$$\ $$ |      $$ | \____$$\ 
    \$$$$  |$$ |      $$ |$$$$$$$  |
     \____/ \__|      \__|\_______/ 
                                    !"#
    );
}

fn display_exit_message(g: GameState) -> () {
    match g {
        GameState::TrisPlayer(i) => print!("\n\n＼(＾O＾)／ Player {i} won.\n"),
        GameState::TrisCom(i) => print!("\n\n[ -c ° ▥ °]-c COM {i} won.\n"),
        GameState::Tie => {
            if rand::thread_rng().gen_bool(0.5) {
                print!("\n\n(◞‸◟；) No one won. It's a tie.\n")
            } else {
                print!("\n\no(╥﹏╥)o No one won. It's a tie.\n")
            }
        }
        _ => panic!("'display_exit_message()' supplied with non-terminal gamestate"),
    }
}

fn display_turn(
    current_turn: &Turn,
    is_com_turn: &bool,
    player_symbols: &Option<&(char, char)>,
) -> () {
    match player_symbols {
        Some(v) => {
            print!(
                "\n\nTurn of {} {} - {} \n",
                if *is_com_turn { "COM" } else { "player" },
                if *current_turn == Turn::Even { 1 } else { 2 },
                if *current_turn == Turn::Even {
                    v.0
                } else {
                    v.1
                }
            )
        }
        None => print!(
            "\n\nTurn of {} {} - {} \n",
            if *is_com_turn { "COM" } else { "player" },
            if *current_turn == Turn::Even { 1 } else { 2 },
            if *current_turn == Turn::Even {
                'X'
            } else {
                'Y'
            }
        ),
    }
}

fn manage_history(board_width: &usize, history_flag: &bool) -> () {
    if *history_flag {
        print!(
            "\n\n{}↓\n\n\n",
            String::from(" ").repeat(board_width * 2 - (!(board_width % 2 == 0)) as usize * 2)
        );
        flush!();
    } else {
        print!("\x1B[2J\x1B[1;1H");
    }
}

fn player_control(board: &mut Board, current_turn: &Turn) -> () {
    let board_width = board[0].len();
    let board_heigth = board.len();

    let x = loop {
        print!("X: ");
        let mut x = String::new();
        flush!();

        if let Result::Err(e) = stdin().read_line(&mut x) {
            println!("Failed to read input: {e}
Try again.");
            continue;
        }

        let x: usize = match x.trim().parse() {
            Ok(v) => v,
            Err(e) => {
                println!("Input is NaN: {e}
Try again.");
                continue;
            }
        };

        if x < 1 || x > board_width {
            continue;
        }
        break x;
    };
    let y = loop {
        print!("Y: ");
        let mut y = String::new();
        flush!();

        if let Result::Err(e) = stdin().read_line(&mut y) {
            println!("Failed to read input: {e}
Try again.");
            continue;
        }

        let y: usize = match y.trim().parse() {
            Ok(v) => v,
            Err(e) => {
                println!("Input is NaN: {e}
Try again.");
                continue;
            }
        };

        if y < 1 || y > board_heigth {
            continue;
        }
        break y;
    };
    if board[y - 1][x - 1] == Cell::Empty {
        board[y - 1][x - 1] = if *current_turn == Turn::Even {
            Cell::PlayerOne
        } else {
            Cell::PlayerTwo
        };
        return;
    } else {
        println!("⚠ Cannot override this tile");
        player_control(board, current_turn);
    }
}

fn com_control(board: &mut Board, current_turn: &Turn, com_move: Point) -> () {
    board[com_move.1][com_move.0] = if *current_turn == Turn::Even {
        Cell::PlayerOne
    } else {
        Cell::PlayerTwo
    };
    print!("X: {}
Y: {}
    ", com_move.0 + 1, com_move.1 + 1);
}

fn com_opp_evaluate(board_to_evaluate: &Board, current_turn: &Turn, tris_magnetism: &u32) -> Point {
    let board_heigth = board_to_evaluate.len();
    let board_width = board_to_evaluate[0].len();
    let mut eval_table = arr![arr![ 1; board_width]; board_heigth];
    let current_player_cell = if *current_turn == Turn::Even {
        Cell::PlayerOne
    } else {
        Cell::PlayerTwo
    };
    let possible_directions = [
        (-1, 1),
        (-1, 0),
        (-1, -1),
        (0, 1),
        (0, 0),
        (0, -1),
        (1, 1),
        (1, 0),
        (1, -1),
    ];

    for &(r_dir, c_dir) in &possible_directions {
        for r in 0..board_heigth {
            for c in 0..board_width {
                if board_to_evaluate[r][c] != Cell::Empty {
                    eval_table[r][c] = 0;
                } else {
                    let r_new = r as i32 + r_dir as i32;
                    let c_new = c as i32 + c_dir as i32;
                    if true
                        && r_new >= 0
                        && r_new < board_to_evaluate.len() as i32
                        && c_new >= 0
                        && c_new < board_to_evaluate[0].len() as i32
                        && board_to_evaluate[r_new as usize][c_new as usize] == current_player_cell
                    {
                        eval_table[r][c] += 1;

                        let r_newer = r as i32 + r_dir as i32 * 2;
                        let c_newer = c as i32 + c_dir as i32 * 2;
                        if cfg![debug_assertions] {
                            println!(
                                "\x1b[96;1mDEBUG\x1b[0m: tris magnetism calcs
    {r} - {r_dir} = {r_new} = {}
    {c} - {c_dir} = {c_new} = {}
    {r} + {r_dir} * 2 = {r_newer} = {}
    {c} + {c_dir} * 2 = {c_newer} = {}",
                                r_new as usize, c_new as usize, r_newer as usize, c_newer as usize
                            );
                        }
                        if true
                            && r_new >= 0
                            && r_new < board_to_evaluate.len() as i32
                            && c_new >= 0
                            && c_new < board_to_evaluate[0].len() as i32
                            && r_newer >= 0
                            && r_newer < board_to_evaluate.len() as i32
                            && c_newer >= 0
                            && c_newer < board_to_evaluate[0].len() as i32
                        {
                            if true
                                && (board_to_evaluate[r_new as usize][c_new as usize]
                                    == current_player_cell)
                                && (board_to_evaluate[r_newer as usize][c_newer as usize]
                                    == current_player_cell)
                            {
                                eval_table[r][c] += tris_magnetism
                            }
                        }
                    }
                }
            }
        }
    }

    let strongest_move = {
        let mut strongest_move = 0;
        for r in &eval_table {
            for c in r {
                if *c > strongest_move {
                    strongest_move = *c;
                }
            }
        }
        strongest_move
    };

    let mut strongest_moves: Arr<Point> = arr![];
    for r in 0..board_heigth {
        for c in 0..board_width {
            if eval_table[r][c] == strongest_move {
                strongest_moves.push(Point(c, r));
            }
        }
    }

    if cfg![debug_assertions] {
        display_eval_process(&eval_table, &strongest_moves)
    };
    strongest_moves[rand::thread_rng().gen_range(0..strongest_moves.len())].clone()
}

fn display_eval_process(eval_table: &Chart, strongest_moves: &Arr<Point>) -> () {
    println!("\x1b[96;1mDEBUG\x1b[0m: eval table");
    for r in 0..eval_table.len() {
        for c in 0..eval_table[0].len() {
            let v = eval_table
                .get(eval_table.len() - r - 1)
                .unwrap()
                .get(c)
                .unwrap();

            print!("{v}");
            if c < eval_table.len() - 1 {
                print!(" │ ")
            }
        }
        if r != eval_table.len() - 1 {
            print!("\n─");
            for _ in 0..eval_table.len() - 1 {
                print!("─┼──");
            }
            print!("\n");
        }
    }

    print!("\n\x1b[96;1mDEBUG\x1b[0m: strongest moves found [ ");
    for i in 0..strongest_moves.len() {
        print!("{}:{}", strongest_moves[i].0, strongest_moves[i].1);
        if i != strongest_moves.len() - 1 {
            print!(",")
        }
        print!(" ")
    }
    print!("]\n");
    flush!();
}