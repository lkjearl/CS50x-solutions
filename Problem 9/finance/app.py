import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT symbol, SUM(shares) as allshares, price FROM orders WHERE userid = ? GROUP BY symbol HAVING SUM(shares) > 0 ORDER BY symbol", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    balance = cash
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["value"] = stock["price"] * stock["allshares"]
        balance += stock["value"]

    return render_template("index.html", stocks=stocks, cash=cash, balance=balance, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    """ CREATE TABLE orders (orderid INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, userid INTEGER, type TEXT NOT NULL, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL, symbol TEXT NOT NULL, price NUMERIC NOT NULL, shares INTEGER NOT NULL, FOREIGN KEY(userid) REFERENCEs users(id));"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be integer")

        if not symbol and not shares:
            return apology("fields cannot be blank")
        elif not lookup(symbol):
            return apology("symbol not found")
        elif shares < 1:
            return apology("shares field must be positive integer")
        else:
            price = lookup(symbol)["price"]
            """ [0]["cash"] makes it not a list, else cash is smth like 'cash: 10,000' """
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
            if cash < (price * shares):
                return apology("cannot afford shares at current price")
            else:
                db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - (price * shares), session["user_id"])
                db.execute("INSERT INTO orders(userid, type, symbol, price, shares) VALUES(?,?,?,?,?)", session["user_id"], 'buy', symbol.upper(), price, shares)
                return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM orders WHERE userid = ? ORDER BY time DESC", session["user_id"])
    return render_template("history.html", history=history, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("symbol field is blank")
        elif not lookup(symbol):
            return apology("symbol not found")

        return render_template("quoted.html", stock=lookup(symbol), usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("username field is blank", 403)
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("passwords fields are blank", 403)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)
        elif len(db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))) != 0:
            return apology("username already exist", 403)
        else:
            hashed = generate_password_hash(request.form.get("password"), method='pbkdf2', salt_length=16)
            db.execute("INSERT INTO users(username, hash) VALUES(?,?)", request.form.get("username"), hashed)
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def changepw():
    """Change user password"""
    if request.method == "POST":
        currentuser = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        if not request.form.get("oldpw") or not request.form.get("newpw") or not request.form.get("confirmation"):
            return apology("fields must not be blank")
        elif request.form.get("newpw") != request.form.get("confirmation"):
            return apology("new passwords do not match")
        elif not check_password_hash(currentuser[0]["hash"], request.form.get("oldpw")):
            return apology("invalid password")
        else:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(request.form.get("newpw"), method='pbkdf2', salt_length=16), session["user_id"])
            return redirect("/logout")
    else:
        return render_template("changepw.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        sumshares = db.execute("SELECT SUM(shares) as allshares FROM orders WHERE userid = ? AND symbol = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"], symbol)
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be integer")

        if not symbol and not shares:
            return apology("fields cannot be blank")
        elif not lookup(symbol):
            return apology("symbol not found")
        elif shares < 1:
            return apology("shares field must be positive integer")
        elif sumshares[0]["allshares"] < shares:
            return apology("user does not own that many shares of the stock")
        else:
            price = lookup(symbol)["price"]
            """ [0]["cash"] makes it not a list, else cash is smth like 'cash: 10,000' """
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + (price * shares), session["user_id"])
            db.execute("INSERT INTO orders(userid, type, symbol, price, shares) VALUES(?,?,?,?,?)", session["user_id"], 'sell', symbol.upper(), price, -shares)
            return redirect("/")
    else:
        symbols = db.execute("SELECT symbol FROM orders WHERE userid = ? GROUP BY symbol HAVING SUM(shares) > 0 ORDER BY symbol", session["user_id"])
    return render_template("sell.html", symbols=symbols)
