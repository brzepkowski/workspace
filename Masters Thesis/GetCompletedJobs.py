#setup
from qiskit import QuantumProgram
import Qconfig

qp = QuantumProgram()
qp.set_api(Qconfig.APItoken, Qconfig.config['url'])

#download details of all the jobs you've ever submitted (well, up to some limit; the default is 50)
my_jobs = qp.get_api().get_jobs(limit=999)

#filter down to get a list of completed jobs
done_jobs = [j for j in my_jobs if j['status']=='COMPLETED']

#print the results for all of your completed jobs
for j in done_jobs:
    for q in j['qasms']:
        print(q['qasm'])
        print(q['result'])
