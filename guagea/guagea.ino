// size of the LED grid
#define GRID_W (8)
#define GRID_H (16)
// max size of each tetris piece
#define PIECE_W (4)
#define PIECE_H (4)

// how many kinds of pieces
#define NUM_PIECE_TYPES (7)

#define JOYSTICK_DEAD_ZONE (30)

#define JOYSTICK_PIN (1)

#include <SoftwareSerial.h>
#include <math.h>
SoftwareSerial s(2, 3);  // 2:RX 3:TX

int latchpin = 5;
int clockpin = 4;
int datapin = 6;

int touch1 = 7;  // 회전용 터치
int touch2 = 8;  // 내리는 터치

int touch_state1 = 0;
int touch_state2 = 0;

int sonic;
// 1 color drawings of each piece in each rotation.
// Each piece is max 4 wide, 4 tall, and 4 rotations.
const char piece_I[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 };
const char piece_L1[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const char piece_L2[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
const char piece_T[] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
const char piece_S1[] = { 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0 };
const char piece_S2[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
const char piece_O[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int row[8] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000
};

int row7[8] = {
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  B00010000,
  B00100000,
  B01000000,
  B10000000
};

int col7[11] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xF6, 0x00 };

int score = 3;
int s_score[4];
int b_score;
int col1[8];
int pcol;
int col2[8];

// An array of pointers!
const char *pieces[NUM_PIECE_TYPES] = {
  piece_S1,
  piece_S2,
  piece_L1,
  piece_L2,
  piece_O,
  piece_T,
  piece_I,
};


//--------------------------------------------------------------------------------
// GLOBALS
//--------------------------------------------------------------------------------

// this is how arduino remembers what the button was doing in the past,
// so arduino can tell when it changes.
int old_button = 0;
// so arduino can tell when user moves sideways
int old_px = 0;
// so arduino can tell when user tries to turn
int old_i_want_to_turn = 0;

// this is how arduino remembers the falling piece.
int piece_id;
int piece_rotation;
int piece_x;
int piece_y;

char piece_sequence[NUM_PIECE_TYPES];
char sequence_i = NUM_PIECE_TYPES;

// this controls how fast the player can move.
long last_move;
long move_delay = 100;  // 100ms = 5 times a second

// this controls when the piece automatically falls.
long last_drop;
long drop_delay = 500;  // 500ms = 2 times a second

// this is how arduino remembers where pieces are on the grid.
char grid[8 * 16];

// 패턴 관련 변수
int pattern_num = 0;  // 패턴 종류 값
int tpattern_num;
unsigned long pattern_lastTime = 0;  // 마지막으로 업데이트 된 시간
const int pattern_interval = 8000;   // 패턴 변환 주기(ms)

//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------



void draw_grid() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      pcol *= 2;
      pcol += grid[8 * j + i];
    }
    col1[i] = pcol;
    pcol = 0;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      pcol *= 2;
      pcol += grid[8 * (j + 8) + i];
    }
    col2[i] = pcol;
    pcol = 0;
  }

  b_score = score;
  for (int i = 0; i < 4; i++) {
    if (b_score == 0) {
      s_score[i] = 10;
    } else {
      s_score[i] = b_score % 10;
      b_score /= 10;
    }
  }
  if (pattern_num == 0) {
    tpattern_num = 0;
  } 
  else if (pattern_num == 1) {
    tpattern_num = 1;
  }
  else if (pattern_num == 2) {
    tpattern_num = 2;
  }
  else if (pattern_num == 3) {
    tpattern_num = 4;
  }
  else if (pattern_num == 4) {
    tpattern_num = 8;
  }
  Serial.println(tpattern_num);
  for (int i = 0; i < 8; i++) {
    digitalWrite(latchpin, LOW);
    shiftOut(datapin, clockpin, LSBFIRST, ~(row7[i] + tpattern_num));
    shiftOut(datapin, clockpin, LSBFIRST, col7[s_score[i % 4]]);
    shiftOut(datapin, clockpin, LSBFIRST, row[i]);
    shiftOut(datapin, clockpin, LSBFIRST, ~col1[i]);
    shiftOut(datapin, clockpin, LSBFIRST, ~col2[i]);
    digitalWrite(latchpin, HIGH);
    delay(1);
    if (pattern_num != 3) delayMicroseconds(1);      // 기본
    if (pattern_num == 3) delayMicroseconds(10000);  // 프레임 감소
  }
}

// choose a new piece from the sequence.
// the sequence is a random list that contains one of each piece.
// that way you're guaranteed an even number of pieces over time,
// tho the order is random.
void choose_new_piece() {
  if (sequence_i >= NUM_PIECE_TYPES) {
    // list exhausted
    int i, j, k;
    for (i = 0; i < NUM_PIECE_TYPES; ++i) {
      do {
        // pick a random piece
        j = rand() % NUM_PIECE_TYPES;
        // make sure it isn't already in the sequence.
        for (k = 0; k < i; ++k) {
          if (piece_sequence[k] == j) break;  // already in sequence
        }
      } while (k < i);
      // not in sequence.  Add it.
      piece_sequence[i] = j;
    }
    // rewind sequence counter
    sequence_i = 0;
  }

  // get the next piece in the sequence.
  piece_id = piece_sequence[sequence_i++];
  // always start the piece top center.
  piece_y = -4;  // -4 squares off the top of the screen.
  piece_x = 3;
  // always start in the same orientation.
  piece_rotation = 0;
}


void erase_piece_from_grid() {
  int x, y;

  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = piece_x + x;
      int ny = piece_y + y;
      if (ny < 0 || ny > GRID_H) continue;
      if (nx < 0 || nx > GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        grid[ny * GRID_W + nx] = 0;  // zero erases the grid location.
      }
    }
  }
}


void add_piece_to_grid() {
  int x, y;

  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = piece_x + x;
      int ny = piece_y + y;
      if (ny < 0 || ny > GRID_H) continue;
      if (nx < 0 || nx > GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        grid[ny * GRID_W + nx] = 1;  // zero erases the grid location.
      }
    }
  }
}


// Move everything down 1 space, destroying the old row number y in the process.
void delete_row(int y) {
  int x;
  for (; y > 0; --y) {
    for (x = 0; x < GRID_W; ++x) {
      grid[y * GRID_W + x] = grid[(y - 1) * GRID_W + x];
    }
  }
  // everything moved down 1, so the top row must be empty or the game would be over.
  for (x = 0; x < GRID_W; ++x) {
    grid[x] = 0;
  }
  score += 8;
}


void remove_full_rows() {
  int x, y, c;
  for (y = 0; y < GRID_H; ++y) {
    // count the full spaces in this row
    c = 0;
    for (x = 0; x < GRID_W; ++x) {
      if (grid[y * GRID_W + x] > 0) c++;
    }
    if (c == GRID_W) {
      // row full!
      delete_row(y);
    }
  }
}

int super_sonic() {
}

void try_to_move_piece_sideways() {

  int new_px = 0;
  if (pattern_num != 2) {
    touch_state1 = digitalRead(touch1);
    touch_state2 = digitalRead(touch2);  // 기본
  }
  if (pattern_num == 2) {
    touch_state1 = digitalRead(touch2);
    touch_state2 = digitalRead(touch1);  // 반전
  }

  if (touch_state1 == HIGH) {
    new_px = 1;
  }
  if (touch_state2 == HIGH) {
    new_px = -1;
  }

  if (new_px != old_px && piece_can_fit(piece_x + new_px, piece_y, piece_rotation) == 1) {
    piece_x += new_px;
  }
  old_px = new_px;
}


void try_to_rotate_piece() {
  int i_want_to_turn = 0;

  // what does the joystick button say
  int new_button = digitalRead(JOYSTICK_PIN);
  // if the button state has just changed AND it is being let go,
  if (new_button > 0 && old_button != new_button) {
    i_want_to_turn = 1;
  }
  old_button = new_button;

  // up on joystick to rotate
  if (sonic == 0) i_want_to_turn = 1;
  if (i_want_to_turn == 1 && i_want_to_turn != old_i_want_to_turn) {
    // figure out what it will look like at that new angle
    int new_pr = (piece_rotation + 1) % 4;
    // if it can fit at that new angle (doesn't bump anything)
    if (piece_can_fit(piece_x, piece_y, new_pr)) {
      // then make the turn.
      piece_rotation = new_pr;
    }
  }
  old_i_want_to_turn = i_want_to_turn;
}


// can the piece fit in this new location?
int piece_can_fit(int px, int py, int pr) {
  if (piece_off_edge(px, py, pr)) return 0;
  if (piece_hits_rubble(px, py, pr)) return 0;
  return 1;
}


int piece_off_edge(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      int ny = py + y;
      if (ny < 0) continue;  // off top, don't care
      if (piece[y * PIECE_W + x] > 0) {
        if (nx < 0) return 1;        // yes: off left side
        if (nx >= GRID_W) return 1;  // yes: off right side
      }
    }
  }

  return 0;  // inside limits
}


int piece_hits_rubble(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    int ny = py + y;
    if (ny < 0) continue;
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      if (piece[y * PIECE_W + x] > 0) {
        if (ny >= GRID_H) return 1;                 // yes: goes off bottom of grid
        if (grid[ny * GRID_W + nx] == 1) return 1;  // yes: grid already full in this space
      }
    }
  }

  return 0;  // doesn't hit
}


void game_over() {
  int x, y;

  while (1) {
    // click the button?
    if (digitalRead(1) == 0) {
      // restart!
      setup();
      score = 0;
      return;
    }
  }
}


void try_to_drop_piece() {
  erase_piece_from_grid();
  if (piece_can_fit(piece_x, piece_y + 1, piece_rotation)) {
    piece_y++;  // move piece down
    add_piece_to_grid();
  } else {
    // hit something!
    // put it back
    add_piece_to_grid();
    remove_full_rows();
    if (game_is_over() == 1) {
      game_over();
    }
    // game isn't over, choose a new piece
    choose_new_piece();
    score += 3;
  }
}


void try_to_drop_faster() {
  if (sonic == 2) {
    // player is holding joystick down, drop a little faster.
    try_to_drop_piece();
  }
}


void react_to_player() {
  erase_piece_from_grid();
  try_to_move_piece_sideways();
  try_to_rotate_piece();
  add_piece_to_grid();

  try_to_drop_faster();
}


// can the piece fit in this new location
int game_is_over() {
  int x, y;
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int ny = piece_y + y;
      int nx = piece_x + x;
      if (piece[y * PIECE_W + x] > 0) {
        if (ny < 0) return 1;  // yes: off the top!
      }
    }
  }

  return 0;  // not over yet...
}
//--------------------------------------------------------------------------------
// 패턴 랜덤 변환 함수
// 0 : 기본  1 : 초음파 반전  2 : 터치센서 반전  3 : 프레임 감소  4 : 모터 작동
//--------------------------------------------------------------------------------
void setting_pattern_random() {
  unsigned long pattern_currentTime = millis();

  if (pattern_currentTime - pattern_lastTime >= pattern_interval) {
    pattern_lastTime = pattern_currentTime;

    pattern_num = random(0, 5);  // 패턴 갯수 설정

    Serial.print("현재 패턴: ");
    if (pattern_num == 0) {
      Serial.print("기본");
    } else if (pattern_num == 1) {
      Serial.print("초음파 반전");
    } else if (pattern_num == 2) {
      Serial.print("터치센서 반전");
    } else if (pattern_num == 3) {
      Serial.print("프레임 감소");
    } else if (pattern_num == 4) {
      Serial.print("모터 작동");
    }
    Serial.println("");
  }
}


// called once when arduino reboots
void setup() {
  int i;

  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  Serial.begin(9600);
  s.begin(9600);

  // set up joystick button
  pinMode(JOYSTICK_PIN, INPUT);
  digitalWrite(JOYSTICK_PIN, HIGH);

  // make sure arduino knows the grid is empty.
  for (i = 0; i < GRID_W * GRID_H; ++i) {
    grid[i] = 0;
  }

  // make the game a bit more random - pull a number from space and use it to 'seed' a crop of random numbers.
  randomSeed(analogRead(5));

  // get ready to start the game.
  choose_new_piece();

  // start the game clock after everything else is ready.
  last_move = millis();
  last_drop = last_move;
}


// called over and over after setup()
void loop() {
  // the game plays at one speed,
  if (millis() - last_move > move_delay) {
    last_move = millis();
    react_to_player();
  }

  // ...and drops the falling block at a different speed.
  if (millis() - last_drop > drop_delay) {
    last_drop = millis();
    try_to_drop_piece();
  }

  // when it isn't doing those two things, it's redrawing the grid.
  draw_grid();
  // 패턴 설정
  setting_pattern_random();

  if (s.available()) {
    char data = s.read();
    sonic = int(data - '0');
    //Serial.println(data);
    if (pattern_num == 1 && sonic == 0) {
      sonic = 2;
    } else if (pattern_num == 1 && sonic == 2) {
      sonic = 0;
    }
  }

  if (pattern_num == 4) {
      s.write('1');
  }
  else {
      s.write('0');
  }
}