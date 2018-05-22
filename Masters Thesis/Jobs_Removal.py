from IBMQuantumExperience import IBMQuantumExperience
api = IBMQuantumExperience("c4813735736e88eee07ce3df48cef33647a44947c5731af8b34229249fafb0b519226ccd49bac89b8c63e53cf17e2dc871a821a8632104b539cf9a1e53f7e58d")

for job in api.get_jobs():
	if job["status"] == "RUNNING":
		print(job, "\n")
		api.cancel_job(id_job=job["id"], hub=None, group=None, project=None, access_token=None, user_id=None)
		# api.get_job(job["id"]).cancel()
