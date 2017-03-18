from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    session["cash"] = db.execute("SELECT cash FROM users WHERE id=:user_id",
        user_id=session["user_id"])[0]["cash"]
    assets = db.execute("SELECT stock,SUM(shares) as 'shares' FROM trades "+ 
        "WHERE user_id=:user_id GROUP BY stock", user_id=session["user_id"])
    session["trades"] = db.execute("SELECT * FROM trades "+ 
        "WHERE user_id=:user_id", user_id=session["user_id"])
        
    session["total"] = session["cash"]
    session["assets"] = []
    for asset in assets:
        if asset["shares"] > 0:
            look = lookup(asset["stock"])
            # convert price to cents
            asset["price"] = int(100*look["price"])
            asset["name"] = look["name"]
            asset["value"] = asset["price"] * asset["shares"]
            session["assets"].append(asset)
            session["total"] += asset["value"]

    return render_template("index.html")

# replaced with /trade
# @app.route("/buy", methods=["GET", "POST"])
# @login_required
# def buy():
#     """Buy shares of stock."""
#     # if user reached route via POST (as by submitting a form via POST)
#     if request.method == "POST":
        
#         # ensure symbol and # shares submitted
#         if not request.form.get("symbol"):
#             return apology("must provide", "stock symbol")
#         elif not request.form.get("shares"):
#             return apology("Must provide", "how many shares")
        
#         session["quote"] = lookup(request.form.get("symbol"))
#         if session["quote"] == None:
#             return apology("Bad Stock Symbol")
        
#         cash = db.execute("SELECT cash FROM users WHERE id = :user_id",\
#             user_id=session["user_id"])[0]["cash"]
#         shares = int(request.form.get("shares"))
#         price = int(100*session["quote"]["price"])
#         # cost, in CENTS (so db can be integer to make life easier)
#         cost = price * shares
#         print(price, shares, cost, cash)
#         # make sure they have enough cash for purchase
#         if cost > cash:
#             return apology("Not enough", "CASH")
#         # update db to reflect purchase
#         print(cash-cost)
#         db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", \
#             cash = cash - cost, user_id=session["user_id"])
#         db.execute("INSERT INTO trades (user_id,stock,shares,price)" +
#             "VALUES (:user_id,:stock,:shares,:price)",
#             user_id=session["user_id"], stock=session["quote"]["symbol"], \
#             shares=shares, price=price)
#         # return user to account page            
#         flash("Purchased {} shares of {} at $ {:.2f} for $ {:.2f}".format(
#             shares, request.form.get("symbol"), \
#             price/100, cost/100))
#         return redirect(url_for('index'))
    
#     # else if user reached route via GET (as by clicking a link or via redirect)
#     else:
#         return render_template("index.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        flash('You were successfully logged in')
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    flash('You were successfully logged out')
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide", "stock symbol")
        
        session["quote"] = lookup(request.form.get("symbol"))
        if session["quote"] == None:
            return apology("Bad Stock Symbol")
        
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("index.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted twice
        elif not request.form.get("password") or not request.form.get("password2"):
            return apology("must provide password twice")
            
        # ensure 1st and 2nd password match
        elif request.form.get("password") != request.form.get("password2"):
            return apology("both passwords must match")

        # ensure username doesn't already exist
        rows = db.execute("SELECT * FROM users WHERE username = :username", \
            username=request.form.get("username"))
        if len(rows) > 0:
            return apology("username already exists")
        
        # add user and hash of password to db
        id = db.execute("INSERT INTO users (username, hash)" 
            "VALUES (:username, :hash)", \
            username=request.form.get("username"), \
            hash=pwd_context.hash(request.form.get("password")))
        
        # remember which user has logged in
        session["user_id"] = id

        # redirect user to home page
        flash('You were successfully registered and logged in')
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

# replace with /trade
# @app.route("/sell", methods=["GET", "POST"])
# @login_required
# def sell():
#     """Sell shares of stock."""
#     return apology("TODO")
    
@app.route("/trade", methods=["GET", "POST"])
@login_required
def trade():
    """Buy/Sell shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure valid symbol and # shares submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")
        elif not request.form.get("shares"):
            return apology("Must provide how many shares")
        elif request.form.get("type") == "sell" \
          and int(request.form.get("shares")) > int(request.form.get("maxshares")):
            return apology("Don't have that many to sell")
        elif int(request.form.get("shares")) <= 0:
            return apology("Must trade more than 0 shares")
            
        session["quote"] = lookup(request.form.get("symbol"))
        if session["quote"] == None:
            return apology("Bad Stock Symbol")
        
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id",\
            user_id=session["user_id"])[0]["cash"]
        shares = int(request.form.get("shares"))
        # price in CENTS to match db $ (integer in db = life easier)
        price = int(100*session["quote"]["price"])
        cost = price * shares

        # adjust cash to reflect buy or sell of shares        
        if request.form.get("type") == "sell":
            cash = cash + cost
            shares = -shares
            txtype = "Sold"
        elif request.form.get("type") == "buy":
            # ensure user can afford purchase
            if cost > cash:
                return apology("Not enough CASH")
            cash = cash - cost
            txtype = "Bought"
        
        # update db (users & trades) to reflect purchase
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", \
            cash=cash, user_id=session["user_id"])
        db.execute("INSERT INTO trades (user_id,stock,shares,price)" +
            "VALUES (:user_id,:stock,:shares,:price)",
            user_id=session["user_id"], stock=request.form.get("symbol"), \
            shares=shares, price=price)
        
        # return user to account page
        flash("{} {} shares of {} at $ {:.2f} for $ {:.2f}".format(
            txtype, abs(shares), request.form.get("symbol"), \
            price/100, cost/100))
        return redirect(url_for('index'))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("index.html")
