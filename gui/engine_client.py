"""Line-based client for the C++ chess engine."""
#генерировал чат гпт надо разобраться 

from dataclasses import dataclass
from pathlib import Path
import subprocess

@dataclass
class EngineResponse:
    board: list
    side_to_move: str
    status: str


class EngineClient:
    def __init__(self, engine_path=None):
        if engine_path is None:
            engine_path = (
                Path(__file__).resolve().parents[1]
                / "engine"
                / "chess_engine.exe"
            )

        self.engine_path = Path(engine_path)
        self.process = None
        self.board = None
        self.side_to_move = None
        self.status = None

    def start(self):
        if self.process is not None:
            raise RuntimeError("Engine is already running")
        if not self.engine_path.is_file():
            raise FileNotFoundError(
                f"Chess engine executable not found: {self.engine_path}"
            )

        self.process = subprocess.Popen(
            [str(self.engine_path)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            text=True,
            encoding="ascii",
            bufsize=1,
        )
        return self._read_response()

    def play_move(self, from_square, to_square):
        if self.process is None or self.process.poll() is not None:
            raise RuntimeError("Engine is not running")
        if not 0 <= from_square < 64 or not 0 <= to_square < 64:
            raise ValueError("Square indices must be between 0 and 63")

        self.process.stdin.write(f"{from_square} {to_square}\n")
        self.process.stdin.flush()
        return self._read_response()

    def _read_response(self):
        line = self.process.stdout.readline()
        if not line:
            return_code = self.process.poll()
            raise RuntimeError(
                f"Chess engine closed its output (exit code {return_code})"
            )

        fields = line.strip().split()
        if fields and fields[0] == "error":
            raise ValueError("Engine rejected command: " + " ".join(fields[1:]))
        if (
            len(fields) != 4
            or fields[0] != "board"
            or len(fields[1]) != 64
            or fields[2] not in ("w", "b")
            or fields[3] not in ("ongoing", "checkmate", "stalemate")
        ):
            raise ValueError(f"Unexpected engine response: {line!r}")

        piece_names = {
            "P": "p",
            "N": "n",
            "B": "b",
            "R": "r",
            "Q": "q",
            "K": "k",
        }
        board = []
        for symbol in fields[1]:
            if symbol == ".":
                board.append(None)
                continue
            piece = piece_names.get(symbol.upper())
            if piece is None:
                raise ValueError(f"Unknown piece symbol: {symbol!r}")
            color = "w" if symbol.isupper() else "b"
            board.append(f"{color}_{piece}")

        self.board = board
        self.side_to_move = fields[2]
        self.status = fields[3]
        return EngineResponse(board.copy(), self.side_to_move, self.status)

    def close(self):
        if self.process is None:
            return

        process = self.process
        self.process = None
        if process.stdin and not process.stdin.closed:
            process.stdin.close()
        try:
            process.wait(timeout=2)
        except subprocess.TimeoutExpired:
            process.terminate()
            process.wait()

    def __enter__(self):
        self.start()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()


def get_initial_board():
    """Return the engine's initial board, closing the process after reading it."""
    with EngineClient() as engine:
        return engine.board.copy()
