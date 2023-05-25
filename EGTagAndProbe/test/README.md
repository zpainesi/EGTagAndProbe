### Crab Submission

#### Multiple crab job submision

For submitting a cms-configuration over multiple dataset `multiCrabSubmission.py` can be used
  - Edit the `multiCrabSubmission.py` to have proper datsets for submission
  -  see `multiCrabSubmission.py -h` for more customization options
Crab setup should be sourced along with the `cmsenv`
```bash
cmsenv
source /cvmfs/cms.cern.ch/common/crab-setup.sh
```

Usage
```bash
python3 multiCrabSubmission.py -i egcheck_calo2022_mod0p5_2023Prelims.py -c submit --ext _hcalZS_t1p0

```
- TODO
   - make the datset also a cmd line option
   - add the possibility of defining lfn , and site in cmdline


