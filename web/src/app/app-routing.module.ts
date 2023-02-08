import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { OverviewComponent } from "src/app/pages/overview/overview.component";
import { AlarmsComponent } from "src/app/pages/alarms/alarms.component";
import { DevToolsComponent } from "src/app/pages/dev-tools/dev-tools.component"; // CLI imports router

const routes: Routes = [
  { path: '', component: OverviewComponent },
  { path: 'alarms', component: AlarmsComponent },
  { path: 'devtools', component: DevToolsComponent }
];

// configures NgModule imports and exports
@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule {
}
