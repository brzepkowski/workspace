# qBraid-tutorial

# Installation

Right now, this repo cannot be installed. You can clone it and use the jupyter notebooks with the python environment. The details for the python environment follow below.

### Python environment

First, make sure you have python3 installed. We would recommend going with `python3.6`. If you are not sure if you have python, go to https://docs.python-guide.org/starting/install3/linux/ for linux and https://docs.python-guide.org/starting/install3/osx/ for mc. This contains resources for checking and installing python3.6. Please install `pip` as well.

Once you have python3.6 and pip are installed, we would recommend working with the envrionments. Environments are the cleanest way for working on a project that has lot of dependencies. python3.6 ships with [venv]() with which you can creat a 'qbraid-env'. `cd` into the folder where you want to keep the files for the python environemnt and then run
```python3 -m venv qbraid-env```
The python environment can be activated using the following command:
```source qbraid-env/bin/activate```
More details could be found [here](https://docs.python.org/3/library/venv.html)


## Requirements
After you have activate your environment, before you install anything, make sure to update your pip with:
```pip install --upgrade pip```
Once you have the python environments up and running, you will need to install all the required packages for qBraid-tutorials. We have included the requirements in the file `requirements.tx`, you can run the following command to install all the packages in one go:
```pip install -r requirements.txt```
This will install everything that is required for the smooth functioning of qBraid-tutorials repo. On opening jupyterlab you may get a prompt for building nbdime, go ahead and click on 'build'. This will get you a button in the toolbar for 'git'. If you make any changes to a notebook, you can click this button, and it will generate nicely formatted git diffs.

To enable ipywidgets in jupyterlab run the following:
```jupyter labextension install @jupyter-widgets/jupyterlab-manager```
With this, you will be able to take the full advantage of the qBraid-quiz repo.

### Installation/plugins with jupyter
`nbstripout` is a great option for clearing the output of the jupyter notebooks. It can be installed using `pip install nbstripout`. For more info see below in [before you commit section](#beforecommit)

`nbdime` is a great tool for looking at the git diff for jupyter notebooks.

For jupyterlab there is a market place extension which you need to enable first and that will let you search and install extensions from within jupyter lab. You can enable the marketplace extension with the following code:

```jupyter labextension install @jupyter-widgets/jupyterlab-manager```

For jupyter notebook, there is a similar extension but that just gets you all the extension in one go and lets you enable or disable them from the jupyter home page toolbar. You can install the extension for the jupyter notebook using:
```pip install jupyter_contrib_nbextensions```

```jupyter contrib nbextension install --user```

### <a name="beforecommit"></a> Before you commit or do a pull request:
Since jupyter is not just a text file and uses JSON format, everytime code/markdown is changed in jupyter notebook, lot of information about the layout changes as well. This is especially the case for python code which outputs pictures/graphs. The pictures are stored as text which show up in the diff. This complicates the git diff. And hence, the best way to version control jupyter notebooks is by clearing the output before doing a commit. We have been using nbstripout for clearing output from notebooks automatically. You can install nbtripout using `pip install nbstripout`. Please make sure to run `nbstripout notebook.ipynb` to clear the output in a file.  To clear the output in all the notebooks in a given folder, you can run it on a folder, e.g. the command `nbstripout Qube/*` clears the output from all the notebooks in `Qube` folder.

