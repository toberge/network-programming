import unittest
from calculator import process

class TestCalculator(unittest.TestCase):

    def test_plus_minus(self):
        self.assertEqual(process('3 4 - 5 +'), 4)
    
    def test_times_minus(self):
        self.assertEqual(process('2 3 * 4 -'), 2)
    
    def test_only_operands(self):
        with self.assertRaises(Exception):
            process('2 3 3')
    
    def test_only_operators(self):
        with self.assertRaises(Exception):
            process('+ - +')

if __name__ == '__main__':
    unittest.main()