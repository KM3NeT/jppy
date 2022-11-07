from pkg_resources import get_distribution, DistributionNotFound
try:
    version = get_distribution(__name__).version
except DistributionNotFound:
    version = "unknown version"

    
from . import pdf_evaluator
from . import constants
from . import oscprob
from . import geane
from . import lang
from . import pdf
from . import npe
