module Interpreter


  (
    -- * Types
    Prog,

    -- * Functions
    evalRaw,
    evalAdt,
  ) where
import Text.Read(readMaybe)
import Data.List(isInfixOf)
import Text.Regex.Posix

-------------------------------------------------------------------------------
--------------------------------- The Expr ADT  -------------------------------
-------------------------------------------------------------------------------
data Expr = Add Expr Expr
          | Sub Expr Expr
          | Mult Expr Expr
          | Equal Expr Expr
          | Smaller Expr Expr
          | Symbol String
          | Value Int deriving (Show, Read)

-- [Optional] TODO Implement a parser for the Expr ADT.
--

-------------------------------------------------------------------------------
---------------------------------- The Prog ADT -------------------------------
-------------------------------------------------------------------------------
data Prog = Eq String Expr
          | Seq Prog Prog
          | If Expr Prog Prog
          | While Expr Prog
          | Return Expr deriving (Show, Read)

-- [Optional] TODO Implement a parser for the Prog ADT.
--


-- [Optional] TODO The *parse* function.  It receives a String - the program in
-- a "raw" format and it could return *Just* a program as an instance of the
-- *Prog* data type if no parsing errors are encountered, or Nothing if parsing
-- failed.
--
-- This is composed with evalAdt to yield the evalRaw function.


filterText str = filter (\ch-> ch /= ' ' && ch /= '\n') str



removeParentheses :: String -> Int -> Int -> String
removeParentheses str i pts = if (str!!0 /= '{' || i >= strlen) then
                                str
                              else
                                if (str!!i == '{') then
                                    removeParentheses str (i+1) (pts+1)
                                else
                                    if (str!!i == '}') then
                                        if (i == strlen-1 && pts == 1) then 
                                            tail (take (strlen-1) str)
                                        else
                                            removeParentheses str (i+1) (pts-1)
                                    else
                                        removeParentheses str (i+1) pts
                              
                              where strlen = length str
                              

-- EXPRESII --
multParser :: String -> Either String Expr
multParser str = _multParser str 0

_multParser str i = if (i >= strlen) then
                        Left "Nu e mult"
                    else
                        if (str!!i == '*') then
                            case op1 of
                                Left a -> op1
                                Right a1 -> case op2 of
                                        Left a -> op2
                                        Right a2 -> Right (Mult (a1) (a2))
                        else
                            _multParser str (i+1)
                    
                      where tup = splitAt i str
                            op1 = exprParser (fst tup) 
                            op2 = exprParser (drop 1 (snd tup))
                            strlen = length str

subParser :: String -> Either String Expr
subParser str = _subParser str 0

_subParser str i = if (i >= strlen) then
                        Left "Nu e sub"
                    else
                        if (str!!i == '-') then
                            case op1 of
                                Left a -> op1
                                Right a1 -> case op2 of
                                        Left a -> op2
                                        Right a2 -> Right (Sub (a1) (a2))
                        else
                            _subParser str (i+1)
                    
                      where tup = splitAt i str
                            op1 = exprParser (fst tup) 
                            op2 = exprParser (drop 1 (snd tup))
                            strlen = length str
                            
                            
addParser :: String -> Either String Expr
addParser str = _addParser str 0

_addParser str i = if (i >= strlen) then
                        Left "Nu e add"
                    else
                        if (str!!i == '+') then
                            case op1 of
                                Left a -> op1
                                Right a1 -> case op2 of
                                        Left a -> op2
                                        Right a2 -> Right (Add (a1) (a2))
                        else
                            _addParser str (i+1)
                    
                      where tup = splitAt i str
                            op1 = exprParser (fst tup) 
                            op2 = exprParser (drop 1 (snd tup))
                            strlen = length str



smaParser :: String -> Either String Expr
smaParser str = _smaParser str 0

_smaParser str i = if (i >= strlen) then
                        Left "Nu e smaller"
                    else
                        if (str!!i == '<') then
                            case op1 of
                                Left a -> op1
                                Right a1 -> case op2 of
                                        Left a -> op2
                                        Right a2 -> Right (Smaller (a1) (a2))
                        else
                            _smaParser str (i+1)
                    
                      where tup = splitAt i str
                            op1 = exprParser (fst tup) 
                            op2 = exprParser (drop 1 (snd tup))
                            strlen = length str

                    
symParser :: String -> Either String Expr
symParser str = if (str =~ "^[a-zA-Z]+$") then -- verific daca respecta numele de variabila
                    Right (Symbol str)
                else
                    Left "Nu e string"
                

valParser :: String -> Either String Expr
valParser str = case (readMaybe str :: Maybe Int) of -- verific daca e int
                    Just a -> Right (Value a)
                    Nothing -> Left "Nu e int"
                              
equalParser :: String -> Either String Expr
equalParser str = _equalParser (str) 0

_equalParser str i = if (i >= strlen-1) then
                        Left "Nu e equal"
                    else
                        if (str!!i == '=' && str!!(i+1) == '=') then -- "=="
                            case op1 of
                                Left a -> op1
                                Right a1 -> case op2 of
                                        Left a -> op2
                                        Right a2 -> Right (Equal (a1) (a2))
                        else
                            _equalParser str (i+1)
                    
                      where tup = splitAt i str
                            op1 = exprParser (fst tup) 
                            op2 = exprParser (drop 2 (snd tup))
                            strlen = length str
                              
exprParser :: String -> Either String Expr
exprParser str = case (rezSym) of
                    Right a -> rezSym
                    Left a -> case (rezVal) of
                                Right a -> rezVal
                                Left a -> case (rezEqual) of
                                            Right a -> rezEqual
                                            Left a -> case (rezSma) of
                                                        Right a -> rezSma
                                                        Left a -> case (rezAdd) of 
                                                            Right a -> rezAdd
                                                            Left a -> case (rezSub) of
                                                                        Right a -> rezSub
                                                                        Left a -> case (rezMult) of
                                                                                    Right a -> rezMult
                                                                                    Left a -> Left "???"
                                
                 where rezSym = symParser str
                       rezVal = valParser str
                       rezEqual = equalParser str
                       rezSma = smaParser str
                       rezAdd = addParser str
                       rezMult = multParser str
                       rezSub = subParser str


                    
-- WHILE --
whileParser :: String -> Either String Prog
whileParser str = if (take 5 str == "while") then
                    _whileParser (drop 5 str) 0 0 
                  else
                    Left str

_whileParser str i pts = if (i >= strlen) then
                            Left "Nu e while"
                         else
                            if (str!!i == '(') then
                                _whileParser str (i+1) (pts+1)
                            else
                                if (str!!i == ')' && pts == 1) then
                                    case (rezExp) of
                                        Left a -> Left a
                                        Right a1 -> __whileParser (drop (i+1) str) 0 0 a1
                                else
                                    _whileParser str (i+1) pts
                                    
                         where strlen = length str
                               rezExp = exprParser (drop 1 (take i str))

__whileParser str i pts a1 = if (i >= strlen) then
                                Left "Nu merge secventa din while"
                            else   
                                if (str!!i == '{') then
                                    __whileParser str (i+1) (pts+1) a1
                                else
                                    if (str!!i == '}' && pts == 1) then
                                        case (rezProg1) of
                                            Left a -> Left a
                                            Right a2 -> if (i + 1 < strlen) then
                                                            case (progContinued) of
                                                                Right a -> Right (Seq (While (a1) (a2)) (a))
                                                                Left a -> Left "Eroare dupa while"
                                                        else
                                                            Right (While (a1) (a2))
                                    else
                                        if (str!!i == '}') then 
                                            __whileParser str (i+1) (pts-1) a1
                                        else
                                            __whileParser str (i+1) pts a1
                                        
                            where strlen = length str
                                  rezProg1 = progParser (drop 1 (take i str))
                                  progContinued = progParser (drop (i+1) str)
                            
                                    
                                    
-- IF --

ifParser :: String -> Either String Prog
ifParser str = if (take 2 str == "if") then
                    _ifParser (drop 2 str) 0 0
                else
                    Left "Nu e if"
                        
                    
-- aici ar trebui sa vina conditia
_ifParser str i pts = if (i >= strlen) then
                        Left "Nu e if"
                      else
                          if (str!!i == '(') then
                            _ifParser str (i+1) (pts+1)
                          else
                            if (str!!i == ')' && pts == 1) then
                                case (rezExp) of
                                    Left a -> Left a
                                    Right a1 -> __ifParser (drop (i+5) str) 0 0 a1
                                    -- drop la tot ce era inainte + length "then"             
                                    
                            else
                                _ifParser str (i+1) pts
                                
                      where strlen = length str
                            rezExp = exprParser (drop 1 (take i str))

-- aici iau secventa de cod din if, pt true          
__ifParser str i pts a1 = if (i >= strlen) then
                            Left "Nu merge secventa pt true"
                          else   
                            if (str!!i == '{') then
                                __ifParser str (i+1) (pts+1) a1
                            else
                                if (str!!i == '}' && pts == 1) then
                                    case (rezProg1) of
                                        Left a -> Left a
                                        Right a2 -> ___ifParser (drop (i+5) str) 0 0 a1 a2  
                                        -- drop la tot ce era inainte + length "else"
                                else
                                    if (str!!i == '}') then
                                        __ifParser str (i+1) (pts-1) a1
                                    else
                                        __ifParser str (i+1) pts a1
                                    
                                    
                        where strlen = length str
                              rezProg1 = progParser (drop 1 (take i str))
                              
-- secventa pt else        
___ifParser str i pts a1 a2 = if (i >= strlen) then
                                Left "Nu merge secventa pe else"
                              else
                                if (str!!i == '{') then
                                    ___ifParser str (i+1) (pts+1) a1 a2
                                else
                                    if (str!!i == '}' && pts == 1) then
                                        case (progParser (drop 1 (take i str))) of
                                            Left a -> Left a
                                            Right a3 -> if (i + 1 < strlen) then
                                                            case (progContinued) of
                                                                Right a -> Right (Seq (If (a1) (a2) (a3)) (a))
                                                                Left a -> Left "Eroare dupa if"
                                                        else
                                                            Right (If (a1) (a2) (a3))
                                    else
                                        if (str!!i == '}') then
                                            ___ifParser str (i+1) (pts-1) a1 a2
                                        else
                                            ___ifParser str (i+1) pts a1 a2
                                
                              where strlen = length str
                                    progContinued = progParser (drop (i+1) str)
                                    -- if-urile nu se separa prin ';'
                                    -- => fix dupa if-ul asta poate urma o alta instructiune
                                    -- care sa nu fie prinsa in splitAt pt ';', deci, verific
                                    -- daca mai sunt caractere ramase dupa ce am procesat programul 'if'
                                    -- daca sunt, intorc o secventa, altfel direct programul respectiv
-- RETURN --
returnParser :: String -> Either String Prog
returnParser str = if (take 6 str == "return") then
                        case exp of 
                            Right a -> Right (Return (a))
                            Left a -> Left "Expresie nevalida"
                    else
                        Left "Nu e return"
                        
                   where exp = exprParser (drop 6 str)

-- EQ --
eqParser :: String -> Either String Prog
eqParser str = _eqParser (removeParentheses (filterText str) 0 0) 0

getString (Symbol s) = s
getString (_) = ""

_eqParser str i = if (i >= strlen) then
                        Left "Nu e eq"
                    else
                        if (str!!i == '=') then
                            case expr1 of
                                Left a -> Left a
                                Right a1 -> case expr2 of
                                            Left a -> Left a
                                            Right a2 -> Right (Eq (getString a1) a2)
                       else
                            _eqParser str (i+1)
                            
                            
                   where tup = splitAt i str
                         expr1 = exprParser (fst tup)
                         expr2 = exprParser (tail (snd tup)) -- sar peste '='
                         strlen = length str
          

-- PROGRAME --
progParser :: String -> Either String Prog
progParser str = _progParser (filterText str)
_progParser str = if (str!!(strlen-1) == ';') then
                    progParser (take (strlen-1) str) -- daca am gasit secventa 
                 else
                 if (str == "") then
                    Left "Nimic scris"
                 else
                     case (rezSeq) of
                        Right a -> rezSeq
                        Left a -> case (rezEq) of
                                       Right a -> rezEq
                                       Left a -> case (rezRet) of
                                                    Right a -> rezRet
                                                    Left a -> case (rezIf) of
                                                                    Right a -> rezIf
                                                                    Left a -> case (rezWhile) of
                                                                                Right a -> rezWhile
                                                                                Left a -> Left "???"
                        
                     where rezSeq = seqParser str
                           rezEq = eqParser str
                           rezRet = returnParser str
                           rezIf = ifParser str
                           rezWhile = whileParser str
                           strlen = length str
                     

                        
                 
-- SEQ --
seqParser :: String -> Either String Prog
seqParser str = _seqParser (removeParentheses (str) 0 0) 0


_seqParser str i =  if (i >= strlen) then
                        Left "Nu e secv"
                    else
                        if (str!!i == ';') then
                            case prog1 of
                                Left a -> prog1
                                Right a1 -> case prog2 of
                                            Left a -> prog2
                                            Right a2 -> Right (Seq a1 a2)
                                            
                         else
                            if (str!!i == '}' && (i + 1) < strlen) then
                            case prog3 of
                                Left a -> prog3
                                Right a1 -> case prog4 of
                                            Left a -> prog4
                                            Right a2 -> Right (Seq a1 a2)
                       else
                            _seqParser str (i+1)
                            
                            
                   where tup = splitAt i str
                         prog1 = progParser (fst tup)
                         prog2 = progParser (drop 1 (snd tup)) -- sar peste ;
                         strlen = length str
                         tup2 = splitAt (i+1) str
                         prog3 = progParser (fst tup)
                         prog4 = progParser (drop 1 (snd tup)) -- sar peste ;




parse :: String -> Maybe Prog
parse str = case (progParser str) of
                Right a -> Just a
                Left a -> Nothing

            
-------------------------------------------------------------------------------
-------------------------------- The Interpreter ------------------------------
-------------------------------------------------------------------------------

-- TODO The *evalAdt* function.  It receives the program as an instance of the
-- *Prog* data type and returns an instance of *Either String Int*; that is,
-- the result of interpreting the program.
--
-- The result of a correct program is always an Int.  This is a simplification
-- we make in order to ease the implementation.  However, we wrap this Int with
-- a *Either String* type constructor to handle errors.  The *Either* type
-- constructor is defined as:
--
-- data Either a b = Left a | Right b
--
-- and it is generally used for error handling.  That means that a value of
-- *Left a* - Left String in our case - wraps an error while a value of *Right
-- b* - Right Int in our case - wraps a correct result (notice that Right is a
-- synonym for "correct" in English).
-- 
-- For further information on Either, see the references in the statement of
-- the assignment.
--



-- < EVALUARE DE EXPRESII > --


type Dictionary = [(String, Int)]
valueof :: String -> (Dictionary) -> Either String Int
valueof var dict = if (length filteredDictionary == 0) then
                        Left "Eroare"
                   else
                        Right (snd (head(filteredDictionary)))
                    
                   where filteredDictionary = (filter (\e->(fst e) == var) dict)


-- ca sa mai scot din case-uri
isRight (Right a) = True
isRight (_) = False
getRight (Right a) = a
getLeft (Left a) = a

evalExp :: Expr -> Dictionary -> Either String Int

evalExp (Value v) dict = Right v

evalExp (Symbol a) dict = if (isRight (valueof a dict)) then 
                                valueof a dict 
                          else 
                                Left "Uninitialized variable"

evalExp (Add a b) dict = if (isRight (resa) && isRight (resb)) then 
                            Right (getRight(resa) + getRight(resb))
                        else
                            if (isRight resa /= True) then
                                resa
                            else
                                resb
                        
                        where resa = evalExp a dict
                              resb = evalExp b dict
    

evalExp (Sub a b) dict = if (isRight (resa) && isRight (resb)) then 
                            Right (getRight(resa) - getRight(resb))
                        else
                            if (isRight resa /= True) then
                                resa
                            else
                                resb
                        
                        where resa = evalExp a dict
                              resb = evalExp b dict

evalExp (Mult a b) dict = if (isRight (resa) && isRight (resb)) then 
                                Right (getRight(resa) * getRight(resb))
                          else
                                if (isRight resa /= True) then
                                    resa
                                else
                                    resb
                        
                          where resa = evalExp a dict
                                resb = evalExp b dict


evalExp (Equal a b) dict = if (isRight (resa) && isRight (resb)) then 
                                if (getRight(resa) == getRight(resb)) then
                                    Right 1
                                else
                                    Right 0
                            else
                                if (isRight resa /= True) then
                                    resa
                                else
                                    resb
                        
                            where resa = evalExp a dict
                                  resb = evalExp b dict


evalExp (Smaller a b) dict = if (isRight (resa) && isRight (resb)) then 
                                if (getRight(resa) < getRight(resb)) then
                                    Right 1
                                else
                                    Right 0
                             else
                                if (isRight resa /= True) then
                                    resa
                                else
                                    resb
                            
                            where resa = evalExp a dict
                                  resb = evalExp b dict

-- </ EVALUARE DE EXPRESII > --                        

-- trebuie sa car dictionaru' ala ca parametru...thanks haskell
-- (Rezultat, Dictionar, Return?)
type ProgInfo = (Either String Int, Dictionary, Int)


getFirst :: (a, b, c) -> a
getFirst (a, b, c) = a

getSecond :: (a, b, c) -> b
getSecond (a, b, c) = b

getThird :: (a, b, c) -> c
getThird (a, b, c) = c

-- wrapper pt eval                        
_evalAdt :: Prog -> Dictionary -> ProgInfo

_evalAdt (Eq str expr) (dict) = case rezEval of
                                    Left a -> (rezEval, dict, 1) -- pot considera ca exista return - oricum prima eroare e asta
                                    Right a -> (rezEval, (str, a):dict, 0) 
                                -- valoarea adaugata ultima data in lista (dictionar) e prima gasita => nu necesita update
                                where rezEval = evalExp expr dict

_evalAdt (Seq prog1 prog2) (dict) = case (getFirst rezEval) of
                                        Left a -> rezEval
                                        Right a -> rezEval2
                                    
                                    where rezEval = _evalAdt prog1 dict
                                          rezEval2 = _evalAdt prog2 (getSecond rezEval)
                                          
_evalAdt (If expr prog1 prog2) (dict) = case rezCond of
                                            Left a -> (rezCond, dict, 1) -- consider ca exista return, deja e eroare pt uninitialized
                                            Right a -> if (a == 1) then
                                                            _evalAdt prog1 dict
                                                       else
                                                            _evalAdt prog2 dict
                                            
                                        where rezCond = evalExp expr dict
                                        
_evalAdt (Return expr) dict = (evalExp expr dict, dict, 1) -- 1 = s-a executat return
                                
_evalAdt (While expr prog) dict = case rezCond of
                                    Left a -> (rezCond, dict, 1)
                                    Right a -> if (a == 1) then
                                                    if (getThird rezItr == 1) then -- exita un return
                                                        rezItr -- rezultatul e rezultatul unei singure iteratii
                                                    else
                                                        case (getFirst rezItr) of -- iau rezultatul unei iteratii
                                                            Left a -> rezItr  -- eroare, nu mai continui
                                                            Right a -> rezNext -- totul ok, continui cu urmatoarea
                                                    
                                               else
                                                    (rezCond, dict, getThird rezItr) -- daca exista return, propag inapoi
                                  
                                  where rezCond = evalExp expr dict
                                        rezItr = _evalAdt prog dict
                                        rezNext = _evalAdt (While expr prog) (getSecond rezItr)
                                        

evalAdt :: Prog -> Either String Int
evalAdt p = if (getThird rezEval == 0) then -- n-are return
                Left "Missing return"
            else
                (getFirst rezEval) -- are return => fie e rezultatul corect fie uninitialized var
                
            where rezEval = _evalAdt p []


-- The *evalRaw* function is already implemented, but it relies on the *parse*
-- function which you have to implement.
--
-- Of couse, you can change this definition.  Only its name and type are
-- important.
evalRaw :: String -> Either String Int
evalRaw rawProg = case parse rawProg of
                    Just prog -> evalAdt prog
                    Nothing   -> Left "Syntax error"
