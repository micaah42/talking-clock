import { Routes } from '@angular/router';


import { OverviewComponent } from "./overview/overview.component";
import { DevToolsComponent } from './dev-tools/dev-tools.component';
import { LightingComponent } from './lighting/lighting.component';
// import { AlarmsComponent } from "./alarms/alarms.component";
// import { DevToolsComponent } from "src/app/pages/dev-tools/dev-tools.component"; // CLI imports router

export const routes: Routes = [
    { path: '', component: OverviewComponent },
    // { path: 'alarms', component: AlarmsComponent },
    { path: 'lighting', component: LightingComponent },
    { path: 'devtools', component: DevToolsComponent }
];
