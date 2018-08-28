function Mcts(state, player) {
  this.parent = null;
  this.children = [];
  this.state = state;
  this.player = player;
  this.move = null;
  this.tried = 0;
  this.win = 0;
}

Mcts.prototype.UCT = function () {
  var root = this;
  while (root.parent) {
    root = root.parent;
  }
  if (this.tried === 0) return 10000;
  return +
    this.win / this.tried +
    Math.sqrt(2 * Math.log(root.tried) / this.tried);
};

Mcts.prototype.select = function () {
  if (this.children.length === 0) return this;
  var i, best = this.children[0], bestuct = best.UCT();
  for (i = 1; i < this.children.length; i++) {
    var uct = this.children[i].UCT();
    if (uct > bestuct) {
      best = this.children[i];
      bestuct = uct;
    }
  }
  return best.select();
};

Mcts.prototype.expand = function () {
  var choice = getNextMoves(this.state, this.player);
  var opponent = 3 - this.player;
  if (choice.length === 0) return ;
  for (var i = 0; i < choice.length; i++) {
    var next = simulateAction(this.state, choice[i], this.player);
    var node = new Mcts(next, opponent);
    node.move = choice[i];
    node.parent = this;
    this.children.push(node);
  }
  var r = (Math.random() * choice.length) | 0;
  return this.children[r];
};

Mcts.prototype.simulate = function () {
  var player = this.player;
  var state = cloneState(this.state);
  while (1) {
    var choice = getNextMoves(state, player);
    if (choice.length === 0) {
      // end game
      return getWinLose(state, player);
    }
    // random move!
    var r = (Math.random() * choice.length) | 0;
    var action = choice[r];
    doAction(state, action, player);
    // next player
    var opponent = 3 - player;
    player = opponent;
  }
};

Mcts.prototype.propagate = function (player) {
  var node = this;
  while (node) {
    if (node.player !== player) {
      node.win++;
    }
    node.tried++;
    node = node.parent;
  }
};
