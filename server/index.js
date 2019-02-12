const express = require('express');
const path = require('path');
const fs = require('fs');
const compression = require('compression');
const helmet = require('helmet');

const app = express();
const PORT = process.env.PORT || 8080;
const DIST = path.join(__dirname, '..', 'dist');

app.enable('trust proxy');

app.use(compression());
app.disable('x-powered-by');
app.use(
  helmet.hsts({
    // Must be at least 18 weeks to be approved by Google
    maxAge: 10886400,
    // Must be enabled to be approved by Google
    includeSubDomains: true,
    preload: true,
  })
);

// upgrade to https if requested on http
// app.get('*', (req, res, next) => {
//   return !req.secure && process.env.NODE_ENV === 'production'
//     ? res.redirect(308, 'https://' + req.headers.host + req.originalUrl)
//     : next();
// });
app.use(express.static(DIST));

app.get('*', (req, res) =>
  fs.createReadStream(path.join(DIST, 'index.html')).pipe(res)
);

app.listen(PORT, () =>
  process.stdout.write(`Tennis server started on port ${PORT}\n`)
);
