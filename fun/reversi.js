window.Int8Array = window.Int8Array || Array;


function getNextMoves(state, player) {
  var action = [];
  for (var i = 0; i < 8; i++) {
    for (var j = 0; j < 8; j++) {
      if (state[i*8+j] === 0) {
        action.push(i*8+j);
      }
    }
  }
  return action;
}

function cloneState(state) {
  var r = new Int8Array(64);
  r.set(state);
  return r;
}

function simulateAction(state, action, player) {
  var next = cloneState(state);
  doAction(next, action, player);
  return next;
}

function doAction(state, action, player) {
  state[action] = player;
  reversiSim(action>>3, action&7, player, state);
}

function getWinLose(state, player) {
  var cntBlack = 0, cntWhite = 0;
  for (var i = 0; i < 8; i++) {
    for (var j = 0; j < 8; j++) {
      if (state[i*8+j] === FIRST_PLAYER) {
        cntBlack++;
      }
      if (state[i*8+j] === SECOND_PLAYER) {
        cntWhite++;
      }
    }
  }
  if (cntBlack > cntWhite) return FIRST_PLAYER;
  if (cntWhite > cntBlack) return SECOND_PLAYER;
  return 0;
}

function reversiSimHelper(i, j, who, board, di, dj) {
  var opponent = 3 - who;
  var y = i, x = j;
  y += di;
  x += dj;
  while (y >= 0 && y < 8 && x >= 0 && x < 8 && board[y*8+x] === opponent) {
    y += di;
    x += dj;
  }
  if (y >= 0 && y < 8 && x >= 0 && x < 8 && board[y*8+x] === who) {
    while (y !== i || x !== j) {
      board[y*8+x] = who;
      y -= di;
      x -= dj;
    }
  }
}

function reversiSim(i, j, who, board) {
  reversiSimHelper(i, j, who, board, 1, 1);
  reversiSimHelper(i, j, who, board, 1, -1);
  reversiSimHelper(i, j, who, board, -1, 1);
  reversiSimHelper(i, j, who, board, -1, -1);
  reversiSimHelper(i, j, who, board, 1, 0);
  reversiSimHelper(i, j, who, board, -1, 0);
  reversiSimHelper(i, j, who, board, 0, 1);
  reversiSimHelper(i, j, who, board, 0, -1);
}
