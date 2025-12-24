-- ============================================
--   Triángulo de Pascal en Haskell
--   Autor: Jeisi Rosales, C.I: 29.668.571
--   Materia: Lenguajes de Programación
-- ============================================

{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use :" #-}

-- Función que genera una fila del triángulo usando recursión
pascalFila :: Int -> [Int]
pascalFila 0 = [1]
pascalFila n = zipWith (+) ([0] ++ anterior) (anterior ++ [0])
    where anterior = pascalFila (n - 1)

-- Genera todas las filas del triángulo hasta n
trianguloPascal :: Int -> [[Int]]
trianguloPascal n = [ pascalFila k | k <- [0..n] ]

-- Imprime el triángulo de forma centrada
imprimirPascal :: Int -> IO ()
imprimirPascal n = mapM_ imprimirFila (zip [0..n] (trianguloPascal n))
    where
        imprimirFila (i, fila) =
            putStrLn (replicate (n - i) ' ' ++ unwords (map show fila))

-- Programa principal
main :: IO ()
main = do
    putStrLn "=== Triángulo de Pascal en Haskell ==="
    putStrLn "¿Cuántas filas deseas generar?"
    entrada <- getLine
    let n = read entrada :: Int
    putStrLn "\nTriángulo generado:\n"
    imprimirPascal n