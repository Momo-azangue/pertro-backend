const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const fs = require('fs');
const cors = require('cors');
const path = require('path');

const app = express();
const port = 3001;

app.use(cors());
app.use(bodyParser.json());

let tasks = [];
let relations = [];
let taskCounter = 0;

const generateTaskLabel = (id) => {
    if (id <= 26) {
        return String.fromCharCode(64 + id); // A-Z
    } else {
        return `T${id - 26}`; // T1, T2, ...
    }
};

app.post('/add-task', (req, res) => {
    const { nom, duree } = req.body;
    taskCounter++;
    const id = taskCounter;
    const label = generateTaskLabel(id);
    tasks.push({ id, label, nom, duree });
    res.status(200).send({ id, label, nom, duree });
});

app.post('/add-relation', (req, res) => {
    const { id, predecesseurs } = req.body;
    const task = tasks.find(task => task.label === id);
    if (!task) {
        return res.status(400).send('Tâche non trouvée');
    }

    predecesseurs.forEach(predecesseur => {
        const predTask = tasks.find(task => task.label === predecesseur);
        if (!predTask) {
            return res.status(400).send(`Prédécesseur ${predecesseur} non trouvé`);
        }
        relations.push({ id: task.id, predecesseur: predTask.id });
    });

    res.status(200).send('Relations ajoutées avec succès');
});

app.get('/calculate-dates', (req, res) => {
    const inputFile = 'input.txt';
    const outputFile = 'output.txt';
    const fictiveTask = { id: 0, nom: 'Fictive', duree: 0 };
    tasks.unshift(fictiveTask);

    let inputContent = `${tasks.length}\n`;
    tasks.forEach(task => {
        inputContent += `${task.id} ${task.nom} ${task.duree}\n`;
    });

    let modifiedRelations = relations.map(relation => {
        return { id: relation.id, predecesseur: relation.predecesseur || 0 };
    });

    inputContent += `${modifiedRelations.length}\n`;
    modifiedRelations.forEach(relation => {
        inputContent += `${relation.id} ${relation.predecesseur}\n`;
    });

    fs.writeFileSync(inputFile, inputContent);

    const pertPath = path.resolve(__dirname, 'C:\\Users\\azang\\CLionProjects\\pertroc\\pert');

    exec(`${pertPath} ${inputFile} ${outputFile}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Erreur : ${error.message}`);
            return res.status(500).send('Erreur du serveur');
        }
        if (stderr) {
            console.error(`Erreur : ${stderr}`);
            return res.status(500).send('Erreur du serveur');
        }

        const resultContent = fs.readFileSync(outputFile, 'utf-8');
        res.send(resultContent);
    });
});

app.listen(port, () => {
    console.log(`Serveur en cours d'exécution sur http://localhost:${port}`);
});
