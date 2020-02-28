import unittest
from calculator import process

class TestCalculator(unittest.TestCase):

    def test_plus_minus(self):
        self.assertEqual(process('3 4 - 5 +'), 4)
    
    def test_times_minus(self):
        self.assertEqual(process('2 3 * 4 -'), 2)
    
    def test_only_operands(self):
        self.assertEqual(process('2 3 3'), 'Too few operators, 2 missing')

if __name__ == '__main__':
    unittest.main()