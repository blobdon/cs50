import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.wordval = {}
        self.wordval.update(load(positives, 1))
        self.wordval.update(load(negatives, -1))

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        for word in text.split():
            score += self.wordval.get(word.lower(), 0)
        return score
        
        
def load(filename, val):
    words = {}
    # with closes the file after the loop
    with open(filename, 'r') as f:
        # for each non-blank line that doesn't begin with ;
        for line in f:
            if line and not line.startswith(';'):
                words[line.strip()] = val
    
    return words